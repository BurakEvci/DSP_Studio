#include "realtimehandler.h"

RealTimeHandler::RealTimeHandler(QObject *parent)
    : QObject{parent}, m_audioSource(nullptr), m_ioDevice(nullptr)
{
}

RealTimeHandler::~RealTimeHandler()
{
    stopMicrophone(); // Uygulama kapanırsa mikrofonu serbest bırak
}

void RealTimeHandler::startMicrophone()
{
    // 1. Cihaz Kontrolü
    const auto device = QMediaDevices::defaultAudioInput();
    if (device.isNull()) {
        qDebug() << "Hata: Mikrofon bulunamadı!";
        return;
    }

    // 2. Ses Formatı (44.1kHz, Mono, 16-bit Int)
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    // 3. AudioSource Başlatma
    m_audioSource = new QAudioSource(device, format, this);
    m_audioSource->setBufferSize(8192); // Gecikmeyi düşürmek için buffer ayarı

    // 4. Dinlemeye Başla
    m_ioDevice = m_audioSource->start();

    // 5. Veri geldikçe processBuffer'ı tetikle
    connect(m_ioDevice, &QIODevice::readyRead, this, &RealTimeHandler::processBuffer);
}

void RealTimeHandler::stopMicrophone()
{
    if (m_audioSource) {
        m_audioSource->stop();
        delete m_audioSource;
        m_audioSource = nullptr;
        m_ioDevice = nullptr;
    }
}

void RealTimeHandler::processBuffer()
{
    if (!m_ioDevice) return;

    // Tampondaki tüm veriyi oku
    QByteArray rawData = m_ioDevice->readAll();

    // Byte verisini 16-bitlik sayılara (qint16) dönüştür
    const qint16 *ptr = reinterpret_cast<const qint16 *>(rawData.constData());
    int sampleCount = rawData.size() / 2; // Her örnek 2 byte

    QVector<double> processedData;
    processedData.reserve(sampleCount); // Performans için yer ayır

    for (int i = 0; i < sampleCount; ++i) {
        // Normalize et (-1.0 ile +1.0 arası)
        double sample = ptr[i] / 32768.0;
        processedData.append(sample);
    }

    // Veriyi paketledik, şimdi MainWindow'a fırlatıyoruz!
    emit dataReady(processedData);
}
