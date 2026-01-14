#ifndef UTILS_H
#define UTILS_H

#include <QVector>
#include <QString>
class Utils
{
public:
    Utils();

    /**
     * @brief saveToWav - Sinyal verisini .wav ses dosyası olarak kaydeder
     * @param data: Ses verisi (-1.0 ile 1.0 arası double değerler)
     * @param sampleRate: Örnekleme hızı (Örn: 44100 Hz)
     * @param fileName: Kaydedilecek dosya yolu
     * @return Başarılıysa true döner
     */
    static bool saveToWav(const QVector<double> &data, int sampleRate, const QString &fileName);
};




#endif // UTILS_H
