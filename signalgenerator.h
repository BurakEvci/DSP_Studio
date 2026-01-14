#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QVector>
#include <cmath>

// Sinyal türlerini belirleyen numaratör
enum class SignalType {
    SINE,       // Sinüs
    SQUARE,     // Kare
    TRIANGLE,   // Üçgen
    SAWTOOTH    // Testere Dişi
};

class SignalGenerator
{
public:
    SignalGenerator();

    /**
     * @brief generateSignal - İstenilen türde sinyal üretir
     * @param type: Sinyal türü (Enum)
     * @param fs: Örnekleme frekansı (Hz)
     * @param duration: Süre (saniye)
     * @param freq: Sinyalin frekansı (Hz)
     * @param amplitude: Sinyalin genliği
     * @param x: Zaman vektörü (Çıktı)
     * @param y: Genlik vektörü (Çıktı)
     */

    static void generateSignal(SignalType type, double fs, double duration,
                               double freq, double amplitude,
                               QVector<double> &x, QVector<double> &y);
};

#endif // SIGNALGENERATOR_H
