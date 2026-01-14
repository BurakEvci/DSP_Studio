#include "noiseprocessor.h"
#include <random> // Daha iyi rastgele sayı üretimi için
#include <ctime>
#include <cmath>
#include <QtMath>

NoiseProcessor::NoiseProcessor() {}

//Sinyale beyaz gürültü veya belirli SNR oranında gürültü ekler

void NoiseProcessor::addNoise(const QVector<double> &input, QVector<double> &output,
                              NoiseType type, double amplitude)
{
    // Çıktı vektörünü hazırla
    output = input; // Önce temiz sinyali kopyala
    int size = input.size();

    // Rastgele sayı üreteci (Seed: şu anki zaman)
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));

    switch (type) {
        case NoiseType::WHITE: {
            // Gaussian (Normal) Dağılım: Ortalaması 0, Standart Sapması 1
            std::normal_distribution<double> distribution(0.0, 1.0);

            for (int i = 0; i < size; ++i) {
                // Sinyal + (RastgeleDeğer * Şiddet)
                output[i] += distribution(generator) * amplitude;
            }
            break;
        }

        case NoiseType::IMPULSE: {
            // Sinyalin rastgele %5'lik kısmına "diken" atalım
            std::uniform_real_distribution<double> dist(0.0, 1.0);

            for (int i = 0; i < size; ++i) {
                double chance = dist(generator);
                if (chance < 0.05) { // %5 olasılıkla
                    // Rastgele pozitif veya negatif büyük bir değer ekle
                    double spike = (dist(generator) > 0.5 ? 1.0 : -1.0) * amplitude * 2.0;
                    output[i] += spike;
                }
            }
            break;
        }

        case NoiseType::SINUSOIDAL: {
            // Sinyalin üzerine yüksek frekanslı (örn: 50Hz gibi) sabit bir gürültü bindir
            // Burada basitlik olsun diye rastgele bir faz kayması ekliyoruz
            double noiseFreq = 20.0; // Varsayılan gürültü frekansı (Bunu parametrik de yapabiliriz)

            for (int i = 0; i < size; ++i) {
                double t = (double)i / size; // Basit zaman adımı
                output[i] += amplitude * qSin(2 * M_PI * noiseFreq * t);
            }
            break;
        }
    }
}
