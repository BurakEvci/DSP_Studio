#ifndef FFTPROCESSOR_H
#define FFTPROCESSOR_H

#include <QVector>
#include <complex>

// UI'daki ComboBox sırasıyla uyumlu olmalı
enum class WindowType {
    RECTANGULAR, // Pencere yok (Ham veri)
    HANN,
    HAMMING,
    BLACKMAN
};

class FFTProcessor
{
public:
    FFTProcessor();

    /**
     * @brief computeFFT - Zaman verisini frekans verisine dönüştürür
     * @param timeSignal: Giriş sinyali (Zaman)
     * @param fs: Örnekleme frekansı (Hz)
     * @param freqAxis: Çıktı - Frekans ekseni (X)
     * @param magnitude: Çıktı - Genlik değerleri (Y)
     * @param window: Uygulanacak pencere fonksiyonu
     */
    static void computeFFT(const QVector<double> &timeSignal, double fs,
                           QVector<double> &freqAxis, QVector<double> &magnitude,
                           WindowType window = WindowType::RECTANGULAR);

private:
    // Yardımcı fonksiyonlar
    static void applyWindow(QVector<double> &signal, WindowType type);
    // Recursive FFT algoritması (Cooley-Tukey)
    static void performFFT(std::vector<std::complex<double>> &data);

};

#endif // FFTPROCESSOR_H
