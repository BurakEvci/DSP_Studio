#ifndef FILTERPROCESSOR_H
#define FILTERPROCESSOR_H

#include <QVector>

// Filtre Türleri Listesi
enum class FilterType {
    MOVING_AVERAGE,
    MEDIAN,
    LOW_PASS,
    // İleride eklenebilecekler:
    HIGH_PASS,
    BAND_STOP,
    KALMAN,
    ALL_PASS,
    BAND_PASS,
    WIENER
};

class FilterProcessor
{
public:
    FilterProcessor();

    /**
     * @brief applyFilter - Seçilen filtreyi sinyale uygular
     * @param input: Gürültülü sinyal
     * @param output: Filtrelenmiş (Temiz) sinyal
     * @param type: Hangi filtre?
     * @param fs: Örnekleme frekansı
     * @param param: Slider'dan gelen değer (Pencere boyutu veya Keskinlik)
     */
    static void applyFilter(const QVector<double> &input, QVector<double> &output,
                            FilterType type, double fs, double param);

    static void applyDelay(const QVector<double> &input, QVector<double> &output,
                           double fs, double delaySeconds, double decay);

private:
    // --- Algoritmalar ---
    static void applyMovingAverage(const QVector<double> &input, QVector<double> &output, int windowSize);
    static void applyMedian(const QVector<double> &input, QVector<double> &output, int windowSize);
    static void applySimpleLowPass(const QVector<double> &input, QVector<double> &output, double alpha);
    static void applyBandStop(const QVector<double> &input, QVector<double> &output, double fs, double frequency);
    static void applyKalman(const QVector<double> &input, QVector<double> &output, double R, double Q);
};


#endif // FILTERPROCESSOR_H
