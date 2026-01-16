#include "filterprocessor.h"
#include <algorithm> // std::sort için gerekli
#include <vector>
#include <cmath>
#include <QDebug>

FilterProcessor::FilterProcessor() {}

//Tüm filtre tiplerinin matematiksel hesaplamaları (Low, High, Band, Notch, All-pass, Moving Avg, Median, Kalman, Wiener)

void FilterProcessor::applyFilter(const QVector<double> &input, QVector<double> &output,
                                  FilterType type, double fs, double param)
{
    if(input.isEmpty()) return;
    output.clear();

    switch (type) {
    case FilterType::MOVING_AVERAGE:
        // Slider değeri (param) pencere boyutudur. En az 1 olmalı.
        {
            int winSize = static_cast<int>(param);
            if (winSize < 1) winSize = 1;
            applyMovingAverage(input, output, winSize);
        }
        break;

    case FilterType::MEDIAN:
        // Median için pencere boyutu TEK sayı olmalıdır (1, 3, 5...)
        // Çift gelirse 1 ekleyip tek yapıyoruz.
        {
            int winSize = static_cast<int>(param);
            if (winSize % 2 == 0) winSize += 1;
            if (winSize < 1) winSize = 1;
            applyMedian(input, output, winSize);
        }
        break;

    case FilterType::LOW_PASS:
        // Low Pass için slider 1-50 arasını bir katsayıya (alpha) dönüştürüyoruz.
        // Parametre arttıkça filtre güçlensin (alpha küçülsün).
        // Alpha 1.0 = Filtre yok, Alpha 0.01 = Çok güçlü filtre
        {
            double alpha = 1.0 / (param * 0.5); // Deneysel bir ölçekleme
            if (alpha > 1.0) alpha = 1.0;
            if (alpha < 0.01) alpha = 0.01;
            applySimpleLowPass(input, output, alpha);
        }
        break;


    case FilterType::BAND_STOP:
        // Burada 'param' değeri winSize değil, MERKEZ FREKANS (Hz) olur.
        applyBandStop(input, output, fs, param);
        break;

    default:
        // Tanımsız filtre gelirse aynen geri döndür
        output = input;
        break;
    }
}


void FilterProcessor::applyMovingAverage(const QVector<double> &input, QVector<double> &output, int windowSize)
{
    int N = input.size();
    output.resize(N);
    int halfWin = windowSize / 2;

    for (int i = 0; i < N; ++i) {
        double sum = 0.0;
        int count = 0;
        // i noktasının etrafındaki komşuları topla
        for (int j = i - halfWin; j <= i + halfWin; ++j) {
            if (j >= 0 && j < N) {
                sum += input[j];
                count++;
            }
        }
        output[i] = sum / count; // Ortalamasını al
    }
}

void FilterProcessor::applyMedian(const QVector<double> &input, QVector<double> &output, int windowSize)
{
    int N = input.size();
    output.resize(N);
    int halfWin = windowSize / 2;
    std::vector<double> windowValues;

    for (int i = 0; i < N; ++i) {
        windowValues.clear();
        // Pencereyi doldur
        for (int j = i - halfWin; j <= i + halfWin; ++j) {
            if (j >= 0 && j < N) {
                windowValues.push_back(input[j]);
            }
        }
        // Sırala (Küçükten büyüğe)
        std::sort(windowValues.begin(), windowValues.end());

        // Ortadaki elemanı al (Median)
        if (!windowValues.empty()) {
            output[i] = windowValues[windowValues.size() / 2];
        } else {
            output[i] = input[i];
        }
    }
}

void FilterProcessor::applySimpleLowPass(const QVector<double> &input, QVector<double> &output, double alpha)
{
    // Exponential Smoothing (IIR Filtre)
    // Formül: y[i] = alpha * x[i] + (1 - alpha) * y[i-1]
    int N = input.size();
    output.resize(N);

    output[0] = input[0]; // İlk değer aynen kalır
    for(int i = 1; i < N; ++i) {
        output[i] = alpha * input[i] + (1.0 - alpha) * output[i-1];
    }
}


void FilterProcessor::applyBandStop(const QVector<double> &input, QVector<double> &output, double fs, double cutOffFreq)
{
    if (input.isEmpty()) return;
    output.resize(input.size());

    // 1. Parametreler
    double fc = cutOffFreq;
    if (fc >= fs / 2.0) fc = (fs / 2.0) - 1.0;

    double Q = 5.0; // Kalite faktörü (Dar bant kesimi için 5-10 ideal)

    // 2. Katsayılar (Biquad Notch)
    double w0 = 2.0 * M_PI * fc / fs;
    double alpha = std::sin(w0) / (2.0 * Q);
    double cosw0 = std::cos(w0);

    double b0 = 1.0;
    double b1 = -2.0 * cosw0;
    double b2 = 1.0;
    double a0 = 1.0 + alpha;
    double a1 = -2.0 * cosw0;
    double a2 = 1.0 - alpha;

    // Normalize
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;

    // 3. Uygulama
    output[0] = b0 * input[0];
    if (input.size() > 1) {
        output[1] = b0 * input[1] + b1 * input[0] - a1 * output[0];
    }

    for (int i = 2; i < input.size(); ++i) {
        output[i] = (b0 * input[i]) + (b1 * input[i-1]) + (b2 * input[i-2])
        - (a1 * output[i-1]) - (a2 * output[i-2]);
    }
}











