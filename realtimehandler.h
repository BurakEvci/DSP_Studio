#ifndef REALTIMEHANDLER_H
#define REALTIMEHANDLER_H

#include <QObject>
#include <QAudioSource>
#include <QMediaDevices>
#include <QIODevice>
#include <QVector>
#include <QDebug>

class RealTimeHandler : public QObject
{
    Q_OBJECT
public:
    explicit RealTimeHandler(QObject *parent = nullptr);
    ~RealTimeHandler();

    // Dışarıdan çağrılacak komutlar
    void startMicrophone();
    void stopMicrophone();

signals:
    // İşlenmiş veriyi MainWindow'a gönderen sinyal
    void dataReady(const QVector<double> &data);

private slots:
    // Mikrofon tamponu doldukça tetiklenecek
    void processBuffer();

private:
    QAudioSource *m_audioSource; // Mikrofon erişim nesnesi
    QIODevice *m_ioDevice;       // Veri akış yolu
};

#endif // REALTIMEHANDLER_H
