#include "fftprocessor.h"
#include <QtMath>
#include <vector>

FFTProcessor::FFTProcessor() {}
//FFT, IFFT, pencereleme (Rectangular, Hamming, Hanning, Blackman) ve ölçekleme (Linear/dB) işlemleri

void FFTProcessor::computeFFT(const QVector<double> &timeSignal, double fs,
                              QVector<double> &freqAxis, QVector<double> &magnitude,
                              WindowType window)
{
    if (timeSignal.isEmpty()) return;

    // 1. Veriyi kopyala ve Pencereleme uygula
    QVector<double> processedSignal = timeSignal;
    applyWindow(processedSignal, window);

    // 2. FFT için 2'nin kuvveti uzunluğuna tamamla (Zero Padding)
    // Bu işlem FFT performansını artırır
    int N = processedSignal.size();
    int powerOfTwo = 1;
    while (powerOfTwo < N) powerOfTwo *= 2;

    // Karmaşık sayı vektörüne çevir
    std::vector<std::complex<double>> complexData(powerOfTwo);
    for (int i = 0; i < N; ++i) {
        complexData[i] = std::complex<double>(processedSignal[i], 0.0);
    }
    // Geri kalan kısımlar zaten 0.0 (Zero Padding)

    // 3. FFT Algoritmasını Çalıştır
    performFFT(complexData);

    // 4. Genlik (Magnitude) Hesapla ve Çıktıyı Hazırla
    // FFT simetriktir, sadece ilk yarıyı (Nyquist frekansına kadar) alacağız.
    int halfSize = powerOfTwo / 2;
    freqAxis.clear();
    magnitude.clear();

    for (int i = 0; i < halfSize; ++i) {
        // Frekans Ekseni Hesaplaması: f = i * Fs / N
        double f = i * fs / powerOfTwo;
        freqAxis.append(f);

        // Genlik Hesaplaması: |z| = sqrt(real^2 + imag^2)
        // Normalize etmek için N'ye bölebiliriz veya 2/N ile çarpabiliriz.
        double mag = std::abs(complexData[i]);

        // Genlik düzeltmesi (Negatif frekansları da hesaba katmak için *2, DC bileşen hariç)
        if (i > 0) mag *= 2.0;
        mag /= N; // Normalizasyon

        magnitude.append(mag);
    }
}

void FFTProcessor::applyWindow(QVector<double> &signal, WindowType type)
{
    int N = signal.size();
    for (int i = 0; i < N; ++i) {
        double multiplier = 1.0;

        // Pencere Formülleri
        switch (type) {
        case WindowType::RECTANGULAR:
            multiplier = 1.0;
            break;
        case WindowType::HANN:
            multiplier = 0.5 * (1.0 - qCos(2.0 * M_PI * i / (N - 1)));
            break;
        case WindowType::HAMMING:
            multiplier = 0.54 - 0.46 * qCos(2.0 * M_PI * i / (N - 1));
            break;
        case WindowType::BLACKMAN:
            multiplier = 0.42 - 0.5 * qCos(2.0 * M_PI * i / (N - 1))
                         + 0.08 * qCos(4.0 * M_PI * i / (N - 1));
            break;
        }
        signal[i] *= multiplier;
    }
}

// Basit Recursive Cooley-Tukey FFT Algoritması
void FFTProcessor::performFFT(std::vector<std::complex<double>> &data)
{
    const size_t N = data.size();
    if (N <= 1) return;

    // Çift ve Tek indisli elemanları ayır
    std::vector<std::complex<double>> even(N / 2);
    std::vector<std::complex<double>> odd(N / 2);

    for (size_t i = 0; i < N / 2; ++i) {
        even[i] = data[2 * i];
        odd[i] = data[2 * i + 1];
    }

    // Recursive çağrı
    performFFT(even);
    performFFT(odd);

    // Birleştir (Butterfly Operation)
    for (size_t k = 0; k < N / 2; ++k) {
        std::complex<double> t = std::polar(1.0, -2.0 * M_PI * k / N) * odd[k];
        data[k] = even[k] + t;
        data[k + N / 2] = even[k] - t;
    }
}
