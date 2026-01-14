#ifndef NOISEPROCESSOR_H
#define NOISEPROCESSOR_H

#include <QVector>

// Gürültü Çeşitleri
enum class NoiseType {
    WHITE,      // Beyaz Gürültü (Gaussian)
    IMPULSE,    // Tuz & Biber (Ani Dikenler)
    SINUSOIDAL  // Periyodik Gürültü (Örn: Şebeke gürültüsü)
};

class NoiseProcessor
{
public:
    NoiseProcessor();

    /**
     * @brief addNoise - Temiz sinyale gürültü ekler
     * @param input: Temiz sinyal (Girdi)
     * @param output: Gürültülü sinyal (Çıktı)
     * @param type: Gürültü türü
     * @param amplitude: Gürültü şiddeti (Katsayı)
     */
    static void addNoise(const QVector<double> &input, QVector<double> &output,
                         NoiseType type, double amplitude);

};

#endif // NOISEPROCESSOR_H
