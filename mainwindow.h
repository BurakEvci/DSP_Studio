#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAudioSink> // Sesi hoparlöre göndermek için (Qt6)
// Qt5 için <QAudioOutput>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QBuffer>      // Sinyali hafızadan okumak için

#include <QMainWindow>
#include "plotmanager.h"
#include "signalgenerator.h"
#include "noiseprocessor.h"
#include "fftprocessor.h"
#include "filterprocessor.h"
#include <QStack>
#include <QVector>
#include <QTimer>
#include "qcustomplot.h"
#include "realtimehandler.h" // Sınıfımızı ekle

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum UndoType {
    UndoFilter, // Filtre işlemini geri almak için
    UndoNoise   // Gürültü işlemini geri almak için
};

struct UndoState {
    QVector<double> signalData; // Sinyalin kendisi
    int sliderValue;            // O anki slider değeri
    // İlerde buraya 'FilterType' bile ekleyebilirsin.
    UndoType type;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddSignal_clicked();
    void on_btnAddNoise_clicked();
    void on_btnClear_clicked();

    void on_cmbFFTScale_currentIndexChanged(int index);

    void on_sliderFilterParam_valueChanged(int value);

    void on_btnMovingAvg_clicked();

    void on_btnMedian_clicked();

    void on_btnLowPass_clicked();

    void on_btnSave_clicked();

    void on_btnLoad_clicked();

    void on_btnUndo_clicked();

    void on_btnBandStop_clicked();

    void on_btnPlayInput_clicked();

    void on_btnPlayOutput_clicked();

    void on_sliderVolume_valueChanged(int value);

    void updatePlayHead(); // Timer her attığında çalışacak fonksiyon

    void on_cmbWindowType_currentIndexChanged(int index);

    void on_chkRealTime_stateChanged(int arg1);
    void updateLivePlot(const QVector<double> &data);

    void on_btnRecord_clicked(); // Buton tıklama olayı

    void updateAllGraphs();
    void plotFFT(const QVector<double> &signal, double fs, QCustomPlot *plot);

    void on_chkRealTime_checkStateChanged(const Qt::CheckState &arg1);

    void on_btnReverb_clicked();

private:
    Ui::MainWindow *ui;

    // Sinyal verileri (Bunlar kalıyor)
    QVector<double> timeVec;
    QVector<double> rawSignal;
    QVector<double> noisySignal;

    // YÖNETİCİLER: Her grafik için bir manager
    PlotManager *m_origTimePlot;  // Orijinal Sinyal (Zaman) Yöneticisi


    PlotManager *m_origFreqPlot;  // Frekans Grafiği Yöneticisi
    // Frekans Verileri
    QVector<double> freqVec; // X ekseni (Hz)
    QVector<double> magVec;  // Y ekseni (Genlik)

    // PlotManager *m_filteredTimePlot;

    // Yardımcı fonksiyon: Hem sinyal ekleyince hem gürültü ekleyince çağıracağız
    void updateFrequencyGraph();


    PlotManager *m_filteredTimePlot; // Filtreli Zaman Grafiği Yöneticisi
    PlotManager *m_filteredFreqPlot; // Filtreli Frekans Grafiği Yöneticisi

    QVector<double> filteredSignal; // Filtrelenmiş Sinyal Verisi


    // Filtre yardımcı fonksiyonu
    void applyAndPlotFilter(FilterType type);

    // Son seçilen filtreyi hafızada tutalım ki slider oynayınca tekrar hesaplayabilelim
    FilterType currentFilterType = FilterType::MOVING_AVERAGE; // Varsayılan

    void updateStats(const QVector<double> &signal);

    // Geçmiş sinyalleri tutan Yığın (Tarihçe)
    QStack<UndoState> undoStack;

    // Ses çalma nesneleri
    QAudioSink *audioSink = nullptr;
    QIODevice *audioDevice = nullptr; // Sesi içine yazacağımız sanal aygıt
    QBuffer *audioBuffer = nullptr;   // Veriyi tutacak tampon bellek
    QByteArray audioBytes;            // Double veriyi Byte'a çevirip burada tutacağız

    // Yardımcı fonksiyonumuz
    void playSignal(const QVector<double> &signal, int sampleRate, int type = 0);

    // Hangi sinyal çalınıyor? (0: Yok, 1: Giriş, 2: Çıkış/Filtreli)
    int currentAudioType = 0;
    void stopAudio();

    QCPItemLine *playHeadLine; // Kırmızı dikey çizgimiz
    QTimer *playHeadTimer;     // Çizgiyi hareket ettiren zamanlayıcı
    QCPItemLine *playHeadLineOutput;

    void updateOutputFFT(); // YENİ: Çıkış FFT grafiğini güncelleyen yardımcı fonksiyon


    RealTimeHandler *m_realTimeHandler;
    QVector<double> m_liveBuffer; // Ekranda göstereceğimiz kayan veri

    bool isRecording = false;

};
#endif // MAINWINDOW_H
