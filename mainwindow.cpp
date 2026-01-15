#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filterprocessor.h"
#include <QMessageBox>
#include <QFileDialog>
#include "utils.h"
#include <QInputDialog>
#include "statsprocessor.h"



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
    // 1. GÜVENLİK KONTROLLERİ
    // EĞER GRAFİK YÖNETİCİLERİ OLUŞMADIYSA DUR (Crash önleyici)
    if (!m_filteredTimePlot || !m_filteredFreqPlot) return;

    // Eğer hiç sinyal yoksa işlem yapma
    if (rawSignal.isEmpty()) return;

    // 2. UI GÜNCELLEMELERİ
    // Son kullanılan filtre türünü kaydet (Slider değişirse lazım olabilir)
    // Son kullanılan filtre türünü kaydet (Slider için lazım)
    currentFilterType = type;

    // Slider Parametresini Al ve LABEL'I GÜNCELLE (Döngüye girmeden!)
    double param = ui->sliderFilterParam->value();

    QString paramName = "Değer";
    if (type == FilterType::MOVING_AVERAGE || type == FilterType::MEDIAN) {
        paramName = "Pencere Boyutu";
    } else if (type == FilterType::LOW_PASS) {
        paramName = "Filtre Gücü";
    } else if (type == FilterType::BAND_STOP) {
        paramName = "Merkez Frekans (Hz)";
    }

    ui->lblSliderValue->setText(QString("%1: %2").arg(paramName).arg(param));


    // Girdi Sinyalini Belirle (Gürültülü yoksa Orijinali al)
    QVector<double> inputSignal;

    if (!filteredSignal.isEmpty()) {
        // Zaten filtrelenmiş veri var, üzerine devam et!
        inputSignal = filteredSignal;
    }
    else if (!noisySignal.isEmpty()) {
        // Henüz filtre yok ama gürültülü sinyal var
        inputSignal = noisySignal;
    }
    else {
        // Gürültü de yok, saf sinyali al
        inputSignal = rawSignal;
    }

    if(inputSignal.isEmpty()) return;


    // 4. FİLTREYİ HESAPLA
    double fs = ui->txtSampleRate->text().toDouble();
    // FilterProcessor içindeki genel applyFilter fonksiyonunu kullanıyoruz
    FilterProcessor::applyFilter(inputSignal, filteredSignal, type, fs, param);


    // 5. FİLTRELİ ZAMAN GRAFİĞİNİ ÇİZ
    m_filteredTimePlot->updatePlot(timeVec, filteredSignal);


    // 6. FİLTRELİ FFT HESAPLA VE ÇİZ
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

    updateStats(filteredSignal);
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
    updateStats(rawSignal);
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
    updateStats(noisySignal);
}

void MainWindow::on_btnClear_clicked()
{
    // 1. Vektörleri (Hafızayı) Temizle
    timeVec.clear();
    rawSignal.clear();
    noisySignal.clear();
    filteredSignal.clear();

    // 2. Grafikleri Temizle
    // PlotManager sınıfımızda 'clearPlot' yazmıştık, şimdi işe yarayacak!

    if(m_origTimePlot) m_origTimePlot->clearPlot();
    if(m_origFreqPlot) m_origFreqPlot->clearPlot();
    if(m_filteredTimePlot) m_filteredTimePlot->clearPlot();
    if(m_filteredFreqPlot) m_filteredFreqPlot->clearPlot();

    // İleride eklenecek diğer grafikleri de buraya ekleyeceğiz
    // if(m_origFreqPlot) m_origFreqPlot->clearPlot();
    // if(m_filteredTimePlot) m_filteredTimePlot->clearPlot();

    // 3. Kullanıcıya bilgi ver (Opsiyonel, Status Bar varsa)
    ui->statusbar->showMessage("Tüm veriler temizlendi.", 3000);

    if(m_origFreqPlot) m_origFreqPlot->clearPlot();

    ui->lblStatMax->setText("0.000");
    ui->lblStatMin->setText("0.000");
    ui->lblStatMean->setText("0.000");
    ui->lblStatRMS->setText("0.000");
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


void MainWindow::on_btnLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Ses Dosyası Yükle", "", "WAV Dosyaları (*.wav)");
    if (fileName.isEmpty()) return;

    // Geçici değişkenler
    QVector<double> loadedData;
    int loadedSampleRate;

    if (Utils::loadFromWav(fileName, loadedData, loadedSampleRate)) {

        // --- SÜRE KISITLAMASI VE KESME ---
        double totalDuration = static_cast<double>(loadedData.size()) / loadedSampleRate;
        int maxSeconds = 30; // Hedef süremiz

        if (totalDuration > maxSeconds) {
            bool ok;
            double startTime = QInputDialog::getDouble(this, "Dosya Çok Uzun",
                                                       QString("Dosya süresi: %1 sn.\nAnaliz için %2 sn alınacak.\nBaşlangıç saniyesi:").arg(totalDuration).arg(maxSeconds),
                                                       0, 0, totalDuration - maxSeconds, 1, &ok);

            if (!ok) startTime = 0; // İptal derse baştan başla

            // Kesme İşlemi
            int startIdx = static_cast<int>(startTime * loadedSampleRate);
            int lengthIdx = maxSeconds * loadedSampleRate;

            // Vektör sınırlarını aşmamak için güvenlik kontrolü (mid fonksiyonu bunu yapsa da garantilemek iyidir)
            if (startIdx + lengthIdx > loadedData.size()) lengthIdx = loadedData.size() - startIdx;

            loadedData = loadedData.mid(startIdx, lengthIdx);

            ui->statusbar->showMessage(QString("Kesilen parça yüklendi: %1.sn - %2.sn").arg(startTime).arg(startTime + maxSeconds), 5000);
        }

        // --- TEMİZLİK VE GÜNCELLEME ---
        // 1. Önceki verileri temizle
        on_btnClear_clicked();

        // 2. Yeni verileri ana değişkenlere aktar
        rawSignal = loadedData;

        // 3. UI'ı güncelle
        ui->txtSampleRate->setText(QString::number(loadedSampleRate));

        // Süreyi hesapla ve yaz
        double duration = static_cast<double>(loadedData.size()) / loadedSampleRate;
        ui->txtDuration->setText(QString::number(duration, 'f', 2));

        // 4. Zaman Vektörünü (X Ekseni) Yeniden Oluştur
        // DÜZELTME: resize yaptıktan sonra 'append' değil '[]' operatörü kullanılmalı.
        // Yoksa vektör boyutu 2 katına çıkar (ilk yarısı boş kalır).
        timeVec.clear();
        timeVec.resize(loadedData.size());
        for (int i = 0; i < loadedData.size(); ++i) {
            timeVec[i] = static_cast<double>(i) / loadedSampleRate;
        }

        // 5. Grafikleri Çiz
        m_origTimePlot->updatePlot(timeVec, rawSignal);

        // --- KRİTİK DÜZELTME: EKSENLERİ MANUEL ZORLA ---
        // Bu kısım, o sıkışma sorununu ve mavi çizgiyi %100 çözer.
        // ui->customPlotTimeOrig diyerek doğrudan grafik nesnesine erişiyoruz.
        ui->customPlotTimeOriginal->xAxis->setRange(0, duration);
        ui->customPlotTimeOriginal->yAxis->setRange(-1.1, 1.1); // Ses sinyali -1..1 arasındadır, biraz pay bıraktık.
        ui->customPlotTimeOriginal->replot();
        // -----------------------------------------------

        // FFT ve İstatistikleri Güncelle
        updateFrequencyGraph();

        updateStats(rawSignal); // İstatistik kodunu yazdığında açabilirsin

        ui->statusbar->showMessage("Dosya başarıyla yüklendi: " + fileName, 5000);

    } else {
        QMessageBox::critical(this, "Hata", "Dosya yüklenemedi! (Sadece 16-bit PCM WAV desteklenir).");
    }
}


void MainWindow::updateStats(const QVector<double> &signal)
{
    // 1. Hesapla
    StatsProcessor::SignalStats stats = StatsProcessor::computeStats(signal);

    // 2. Ekrana Yaz (Virgülden sonra 3 hane hassasiyetle)
    // Label isimlerinin Qt Designer'dakiyle AYNI olduğundan emin ol!
    ui->lblStatMax->setText(QString::number(stats.maxVal, 'f', 3));
    ui->lblStatMin->setText(QString::number(stats.minVal, 'f', 3));
    ui->lblStatMean->setText(QString::number(stats.meanVal, 'f', 3));
    ui->lblStatRMS->setText(QString::number(stats.rmsVal, 'f', 3));
}


