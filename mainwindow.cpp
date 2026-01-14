#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ComboBox doldurma SignalType
    ui->cmbSignalType->addItems({"Sinüs", "Kare", "Üçgen", "Testere Dişi"});

    // ComboBox doldurma NoiseType
    ui->cmbNoiseType->addItems({"White Noise", "Impulse Noise", "Sinusoidal Noise"});

    // ComboBox doldurma WindowType
    ui->cmbWindowType->addItems({"Rectangular", "Hann", "Hamming", "Blackman"});

    // --- MODÜLER BAĞLANTI ---
    // PlotManager'ı oluşturup, ona UI'daki "customPlotTimeOriginal"i teslim ediyoruz.
    m_origTimePlot = new PlotManager(ui->customPlotTimeOriginal);
    m_origTimePlot->setupPlot("Orijinal Sinyal", "Zaman (s)", "Genlik");


    // PlotManager'ı oluşturup, ona UI'daki "customPlotFreqOriginal"i teslim ediyoruz.
    m_origFreqPlot = new PlotManager(ui->customPlotFreqOriginal);
    m_origFreqPlot->setupPlot("Frekans Spektrumu", "Frekans (Hz)", "Genlik");


}

MainWindow::~MainWindow()
{
    delete m_origTimePlot;
    delete ui;
}


void MainWindow::updateFrequencyGraph()
{
    // Hangi sinyali çizeceğiz? Gürültülü varsa onu, yoksa ham sinyali.
    QVector<double> signalToProcess;
    if (!noisySignal.isEmpty()) {
        signalToProcess = noisySignal;
    } else if (!rawSignal.isEmpty()) {
        signalToProcess = rawSignal;
    } else {
        return; // Veri yok
    }

    double fs = ui->txtSampleRate->text().toDouble();

    // Pencere tipini UI'dan al
    WindowType wType = static_cast<WindowType>(ui->cmbWindowType->currentIndex());

    // FFT Hesapla
    FFTProcessor::computeFFT(signalToProcess, fs, freqVec, magVec, wType);

    // Grafiği Güncelle
    m_origFreqPlot->updatePlot(freqVec, magVec);
}


void MainWindow::on_btnAddSignal_clicked()
{
    // 1. UI'dan Verileri Al (Değişmedi)
    if(ui->txtSampleRate->text().isEmpty() /* ...diğer kontroller... */) return;

    double fs = ui->txtSampleRate->text().toDouble();
    double duration = ui->txtDuration->text().toDouble();
    double freq = ui->txtSignalFreq->text().toDouble();
    double amplitude = ui->dsbSignalAmplitude->value();
    SignalType type = static_cast<SignalType>(ui->cmbSignalType->currentIndex());

    // 2. Sinyali Üret (Değişmedi)
    SignalGenerator::generateSignal(type, fs, duration, freq, amplitude, timeVec, rawSignal);

    // 3. GRAFİĞİ ÇİZDİR (Artık tek satır!)
    m_origTimePlot->updatePlot(timeVec, rawSignal);

    updateFrequencyGraph();
}


void MainWindow::on_btnAddNoise_clicked()
{
    // 1. Önce sinyal var mı kontrol et?
    if (rawSignal.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Önce bir sinyal oluşturmalısınız!");
        return;
    }

    // 2. UI parametrelerini al
    double noiseAmp = ui->dsbNoiseAmplitude->value();
    NoiseType type = static_cast<NoiseType>(ui->cmbNoiseType->currentIndex());

    // 3. Gürültü Ekle (Temiz 'rawSignal'i al, 'noisySignal'e yaz)
    NoiseProcessor::addNoise(rawSignal, noisySignal, type, noiseAmp);

    // 4. Grafiği Güncelle
    // Burada kullanıcı kirli sinyali görsün diye TimePlot'u güncelliyoruz
    m_origTimePlot->updatePlot(timeVec, noisySignal);

    updateFrequencyGraph();
}

void MainWindow::on_btnClear_clicked()
{
    // 1. Vektörleri (Hafızayı) Temizle
    timeVec.clear();
    rawSignal.clear();
    noisySignal.clear();

    // 2. Grafikleri Temizle
    // PlotManager sınıfımızda 'clearPlot' yazmıştık, şimdi işe yarayacak!
    if(m_origTimePlot) {
        m_origTimePlot->clearPlot();
    }

    // İleride eklenecek diğer grafikleri de buraya ekleyeceğiz
    // if(m_origFreqPlot) m_origFreqPlot->clearPlot();
    // if(m_filteredTimePlot) m_filteredTimePlot->clearPlot();

    // 3. Kullanıcıya bilgi ver (Opsiyonel, Status Bar varsa)
    ui->statusbar->showMessage("Tüm veriler temizlendi.", 3000);

    if(m_origFreqPlot) m_origFreqPlot->clearPlot();
}


//Kullanıcı arayüzü, signal-slot yönetimi, buton event’leri
