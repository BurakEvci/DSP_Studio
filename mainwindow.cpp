#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filterprocessor.h"
#include <QMessageBox>
#include <QFileDialog>
#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- MODÜLER BAĞLANTI ---
    // PlotManager'ı oluşturup, ona UI'daki "customPlotTimeOriginal"i teslim ediyoruz.
    m_origTimePlot = new PlotManager(ui->customPlotTimeOriginal);
    m_origTimePlot->setupPlot("Orijinal Sinyal", "Zaman (s)", "Genlik");


    // PlotManager'ı oluşturup, ona UI'daki "customPlotFreqOriginal"i teslim ediyoruz.
    m_origFreqPlot = new PlotManager(ui->customPlotFreqOriginal);
    m_origFreqPlot->setupPlot("Frekans Spektrumu", "Frekans (Hz)", "Genlik");


    // PlotManager'ı oluşturup, ona UI'daki "customPlotTimeOriginal"i teslim ediyoruz.
    m_filteredTimePlot = new PlotManager(ui->customPlotTimeFiltered);
    m_filteredTimePlot->setupPlot("Filtrelenmiş Sinyal", "Zaman (s)", "Genlik");

    // PlotManager'ı oluşturup, ona UI'daki "customPlotFreqOriginal"i teslim ediyoruz.
    m_filteredFreqPlot = new PlotManager(ui->customPlotFreqFiltered);
    m_filteredFreqPlot->setupPlot("Filtre Sonrası Spektrum", "Frekans (Hz)", "Genlik");


    // ComboBox doldurma SignalType
    ui->cmbSignalType->addItems({"Sinüs", "Kare", "Üçgen", "Testere Dişi"});
    // ComboBox doldurma NoiseType
    ui->cmbNoiseType->addItems({"White Noise", "Impulse Noise", "Sinusoidal Noise"});
    // ComboBox doldurma WindowType
    ui->cmbWindowType->addItems({"Rectangular", "Hann", "Hamming", "Blackman"});
    // ComboBox doldurma WindowType
    ui->cmbFFTScale->addItems({"Lineer", "dB (Logaritmik)"});

    // Slider Başlangıç Ayarları
    ui->sliderFilterParam->setRange(1, 50); // En az 1, en çok 50
    ui->sliderFilterParam->setValue(5);     // Başlangıç değeri
    ui->lblSliderValue->setText("Değer: 5");

    // Varsayılan olarak Moving Avg seçili gibi davranalım
    currentFilterType = FilterType::MOVING_AVERAGE;
}

MainWindow::~MainWindow()
{
    // Oluşturduğumuz her şeyi siliyoruz
    if(m_origTimePlot) delete m_origTimePlot;
    if(m_origFreqPlot) delete m_origFreqPlot;
    if(m_filteredTimePlot) delete m_filteredTimePlot;
    if(m_filteredFreqPlot) delete m_filteredFreqPlot;

    //delete m_origTimePlot;
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

    // YENİ: Ölçekleme Kontrolü
    // ComboBox'ta 0: Lineer, 1: dB
    bool isDB = (ui->cmbFFTScale->currentIndex() == 1);
    FFTProcessor::applyMagnitudeScaling(magVec, isDB);



    // Grafiği Güncelle
    // Eğer dB seçiliyse Y ekseni etiketini güncelleyeceğiz
    if (isDB)
        m_origFreqPlot->setupPlot("Frekans Spektrumu", "Frekans (Hz)", "Genlik (dB)");
    else
        m_origFreqPlot->setupPlot("Frekans Spektrumu", "Frekans (Hz)", "Genlik");

    m_origFreqPlot->updatePlot(freqVec, magVec);
}

void MainWindow::applyAndPlotFilter(FilterType type)
{
    // EĞER GRAFİK YÖNETİCİLERİ OLUŞMADIYSA DUR (Crash önleyici)
    if (!m_filteredTimePlot || !m_filteredFreqPlot) return;

    // Son kullanılan filtre türünü kaydet (Slider için lazım)
    currentFilterType = type;

    // Slider Parametresini Al ve LABEL'I GÜNCELLE (Döngüye girmeden!)
    double param = ui->sliderFilterParam->value();

    QString paramName = "Değer";
    if (type == FilterType::MOVING_AVERAGE || type == FilterType::MEDIAN) {
        paramName = "Pencere Boyutu";
    } else if (type == FilterType::LOW_PASS) {
        paramName = "Filtre Gücü";
    }
    ui->lblSliderValue->setText(QString("%1: %2").arg(paramName).arg(param));


    // Girdi Sinyalini Belirle (Gürültülü yoksa Orijinali al)
    QVector<double> inputSignal = noisySignal.isEmpty() ? rawSignal : noisySignal;
    if(inputSignal.isEmpty()) return;

    double fs = ui->txtSampleRate->text().toDouble();

    // 1. Filtreyi Hesapla
    FilterProcessor::applyFilter(inputSignal, filteredSignal, type, fs, param);

    // 2. Filtreli Zaman Grafiğini Çiz
    m_filteredTimePlot->updatePlot(timeVec, filteredSignal);

    // 3. Filtreli FFT'yi Hesapla ve Çiz
    QVector<double> freqAxis, magVec;
    // WindowType'ı UI'dan al
    WindowType wType = static_cast<WindowType>(ui->cmbWindowType->currentIndex());

    FFTProcessor::computeFFT(filteredSignal, fs, freqAxis, magVec, wType);

    // dB dönüşümü isteniyor mu?
    bool isDB = (ui->cmbFFTScale->currentIndex() == 1); // 1: dB, 0: Lineer
    FFTProcessor::applyMagnitudeScaling(magVec, isDB);

    // Başlığı güncelle
    if (isDB)
        m_filteredFreqPlot->setupPlot("Filtre Sonrası Spektrum", "Frekans (Hz)", "Genlik (dB)");
    else
        m_filteredFreqPlot->setupPlot("Filtre Sonrası Spektrum", "Frekans (Hz)", "Genlik");

    m_filteredFreqPlot->updatePlot(freqAxis, magVec);
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

void MainWindow::on_cmbFFTScale_currentIndexChanged(int index)
{
    updateFrequencyGraph();
}


void MainWindow::on_sliderFilterParam_valueChanged(int value)
{
    // 1. Label'ı anlık güncelle (Bağlam Duyarlı)
    QString paramName = "Değer";
    if (currentFilterType == FilterType::MOVING_AVERAGE || currentFilterType == FilterType::MEDIAN) {
        paramName = "Pencere Boyutu";
    } else if (currentFilterType == FilterType::LOW_PASS) {
        paramName = "Filtre Gücü";
    }

    ui->lblSliderValue->setText(QString("%1: %2").arg(paramName).arg(value));

    // 2. Slider oynadıkça filtreyi ANLIK olarak uygula (Opsiyonel ama çok havalı olur)
    // Eğer performans sorunu olursa bu satırı yorum satırı yapabilirsin.
    applyAndPlotFilter(currentFilterType);
}


void MainWindow::on_btnMovingAvg_clicked(){
    applyAndPlotFilter(FilterType::MOVING_AVERAGE);
}

void MainWindow::on_btnMedian_clicked(){
    applyAndPlotFilter(FilterType::MEDIAN);
}

void MainWindow::on_btnLowPass_clicked(){
    applyAndPlotFilter(FilterType::LOW_PASS);
}


void MainWindow::on_btnSave_clicked()
{
    // 1. Kaydedilecek veriyi seç
    // Öncelik: Filtrelenmiş > Gürültülü > Orijinal
    QVector<double> dataToSave;
    if (!filteredSignal.isEmpty()) {
        dataToSave = filteredSignal;
    } else if (!noisySignal.isEmpty()) {
        dataToSave = noisySignal;
    } else if (!rawSignal.isEmpty()) {
        dataToSave = rawSignal;
    } else {
        QMessageBox::warning(this, "Hata", "Kaydedilecek sinyal yok!");
        return;
    }

    // 2. Kullanıcıya nereye kaydedeceğini sor
    QString fileName = QFileDialog::getSaveFileName(this, "Sesi Kaydet", "", "WAV Dosyası (*.wav)");
    if (fileName.isEmpty()) return; // İptal ettiyse çık

    // 3. Örnekleme hızını al
    // Eğer kutu boşsa varsayılan 44100 olsun
    int fs = ui->txtSampleRate->text().toInt();
    if(fs <= 0) fs = 44100;

    // 4. Kaydet
    if (Utils::saveToWav(dataToSave, fs, fileName)) {
        ui->statusbar->showMessage("Dosya başarıyla kaydedildi: " + fileName, 5000);
    } else {
        QMessageBox::critical(this, "Hata", "Dosya kaydedilemedi!");
    }
}

