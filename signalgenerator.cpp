#include "signalgenerator.h"
#include <QtMath> // M_PI ve qSin kullanımı için

SignalGenerator::SignalGenerator() {}
//Sinüs sinyali üretir, zaman vektörü oluşturur, toplama işlemleri yapar

void SignalGenerator::generateSignal(SignalType type, double fs, double duration,
                                     double freq, double amplitude,
                                     QVector<double> &x, QVector<double> &y)
{
    // 1. Örnek sayısını hesapla
    int sampleCount = static_cast<int>(fs * duration);

    // 2. Vektör boyutlarını ayarla (Performans için resize şart)
    x.resize(sampleCount);
    y.resize(sampleCount);

    // 3. Döngü ile sinyali oluştur
    for (int i = 0; i < sampleCount; ++i) {
        double t = i / fs; // Zaman (saniye cinsinden)
        double val = 0.0;
        double omega = 2.0 * M_PI * freq * t; // Açısal frekans

        switch (type) {
        case SignalType::SINE:
            // Standart Sinüs: A * sin(wt)
            val = amplitude * qSin(omega);
            break;

        case SignalType::SQUARE:
            // Kare Dalga: Sinüs pozitifse A, negatifse -A
            val = (qSin(omega) >= 0) ? amplitude : -amplitude;
            break;

        case SignalType::TRIANGLE:
            // Üçgen Dalga: Matematiksel formülü (ArcSin kullanarak)
            // (2*A / PI) * asin(sin(wt))
            val = (2.0 * amplitude / M_PI) * qAsin(qSin(omega));
            break;

        case SignalType::SAWTOOTH:
            // Testere Dişi: A * (2 * (t*f - floor(t*f + 0.5)))
            // Basitleştirilmiş versiyon: atan(tan(wt/2)) veya fmod kullanımı
            // Buradaki formül periyodik testere dişi için:
            {
                double period = 1.0 / freq;
                double cyclePos = fmod(t, period); // 0 ile periyot arası
                // -A'dan +A'ya lineer artış
                val = 2 * amplitude * (cyclePos / period) - amplitude;
            }
            break;
        }

        x[i] = t;
        y[i] = val;
    }
}
