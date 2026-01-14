#include "utils.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>

Utils::Utils() {}

//Ortak fonksiyonlar: normalize(), convertToDb(), magnitudeSpectrum(), windowCoefficients(), vs.

bool Utils::saveToWav(const QVector<double> &data, int sampleRate, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream out(&file);
    out.setByteOrder(QDataStream::LittleEndian); // WAV formatı Little Endian kullanır

    // --- 1. WAV HEADER HAZIRLIĞI ---
    int channelCount = 1;     // Mono
    int sampleSize = 16;      // 16-bit
    int byteRate = sampleRate * channelCount * (sampleSize / 8);
    int blockAlign = channelCount * (sampleSize / 8);
    int dataChunkSize = data.size() * (sampleSize / 8);
    int fileSize = 36 + dataChunkSize; // Header (36) + Data

    // RIFF Chunk
    out.writeRawData("RIFF", 4);
    out << static_cast<quint32>(fileSize);
    out.writeRawData("WAVE", 4);

    // Format Chunk
    out.writeRawData("fmt ", 4);
    out << static_cast<quint32>(16); // Format chunk size (PCM için 16)
    out << static_cast<quint16>(1);  // Audio format (1 = PCM)
    out << static_cast<quint16>(channelCount);
    out << static_cast<quint32>(sampleRate);
    out << static_cast<quint32>(byteRate);
    out << static_cast<quint16>(blockAlign);
    out << static_cast<quint16>(sampleSize);

    // Data Chunk
    out.writeRawData("data", 4);
    out << static_cast<quint32>(dataChunkSize);

    // --- 2. VERİ DÖNÜŞÜMÜ VE YAZMA ---
    // Double (-1.0 to 1.0) -> Int16 (-32768 to 32767)
    for (double sample : data) {
        // Clipping (Sınırlandırma): Sinyal 1.0'ı geçerse cızırtı yapar, kesiyoruz.
        if (sample > 1.0) sample = 1.0;
        if (sample < -1.0) sample = -1.0;

        // Dönüştürme
        qint16 pcmSample = static_cast<qint16>(sample * 32767);
        out << pcmSample;
    }

    file.close();
    return true;
}
