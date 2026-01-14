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

    // IFFT (Ters Dönüşüm)
    static void computeIFFT(const QVector<double> &freqReal, const QVector<double> &freqImag,
                            QVector<double> &timeSignal);

    // YENİ: dB Dönüşümü Yardımcısı
    // isDB: true ise dB'ye çevirir, false ise dokunmaz (Linear kalır)
    static void applyMagnitudeScaling(QVector<double> &magnitude, bool isDB);


private:
    // Yardımcı fonksiyonlar
    static void applyWindow(QVector<double> &signal, WindowType type);
    // Recursive FFT algoritması (Cooley-Tukey)
    static void performFFT(std::vector<std::complex<double>> &data, bool inverse = false);

};

#endif // FFTPROCESSOR_H
