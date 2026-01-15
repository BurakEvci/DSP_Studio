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


bool Utils::loadFromWav(const QString &fileName, QVector<double> &data, int &sampleRate)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    // --- HEADER KONTROLLERİ ---
    char chunkId[4];
    in.readRawData(chunkId, 4);
    if (strncmp(chunkId, "RIFF", 4) != 0) return false;

    in.skipRawData(4); // File Size

    char format[4];
    in.readRawData(format, 4);
    if (strncmp(format, "WAVE", 4) != 0) return false;

    char subchunk1Id[4];
    in.readRawData(subchunk1Id, 4); // "fmt "

    quint32 subchunk1Size;
    in >> subchunk1Size;

    quint16 audioFormat;
    in >> audioFormat;
    if (audioFormat != 1) return false; // Sadece PCM destekliyoruz (Sıkıştırılmamış)

    quint16 numChannels;
    in >> numChannels;

    quint32 fs;
    in >> fs;
    sampleRate = static_cast<int>(fs);

    in.skipRawData(6); // ByteRate + BlockAlign

    quint16 bitsPerSample;
    in >> bitsPerSample;

    // Sadece 16-bit destekleyelim (Şimdilik)
    if (bitsPerSample != 16) return false;

    // fmt chunk'ı beklenenden uzunsa geri kalanını atla (Bazen metadata olur)
    if (subchunk1Size > 16) {
        in.skipRawData(subchunk1Size - 16);
    }

    // --- DATA CHUNK ARAMA ---
    // Bazen "LIST" veya başka chunklar araya girebilir, "data"yı bulana kadar git.
    while (true) {
        if (in.atEnd()) return false;

        char header[4];
        if (in.readRawData(header, 4) < 4) return false;

        quint32 size;
        in >> size;

        if (strncmp(header, "data", 4) == 0) {
            // data bulundu! size değişkeni veri boyutunu tutuyor.

            // --- PERFORMANS OPTİMİZASYONU ---
            // Kaç örnek (sample) okuyacağımızı hesaplayalım
            // 16 bit = 2 byte.
            int bytesPerSample = 2;
            int totalSamples = size / (bytesPerSample * numChannels);

            data.clear();
            data.resize(totalSamples); // Vektörü TEK SEFERDE büyüt (Çok hızlandırır)

            for (int i = 0; i < totalSamples; ++i) {
                qint16 sampleL;
                in >> sampleL;

                double val = 0.0;

                if (numChannels == 1) {
                    // Mono
                    val = static_cast<double>(sampleL) / 32768.0;
                }
                else if (numChannels == 2) {
                    // Stereo: Sağ kanalı da oku ve ortalamasını al
                    qint16 sampleR;
                    in >> sampleR;
                    val = (static_cast<double>(sampleL) + static_cast<double>(sampleR)) / 2.0;
                    val /= 32768.0; // Normalize et
                }

                // Vektöre yaz (append kullanmıyoruz, [] operatörü daha hızlı)
                data[i] = val;
            }

            break; // Döngüden çık
        } else {
            // data değilse atla
            in.skipRawData(size);
        }
    }

    file.close();
    return !data.isEmpty();
}


