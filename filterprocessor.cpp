#include "filterprocessor.h"
#include <algorithm> // std::sort için gerekli
#include <vector>
#include <cmath>

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
