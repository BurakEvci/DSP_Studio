#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filterprocessor.h"
#include <QMessageBox>
#include <QFileDialog>
#include "utils.h"
#include <QInputDialog>
#include "statsprocessor.h"
#include <QStyle>


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
    } else if (type == FilterType::LOW_PASS || type == FilterType::HIGH_PASS) {
        paramName = "Kesim Frekansı (Hz)";
    } else if (type == FilterType::BAND_STOP || type == FilterType::BAND_PASS) {
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
        // Henüz filtre yok ama gürültülü sinyal var, İlk kez filtreliyorsak gürültülü sinyali al
        inputSignal = noisySignal;
    }
    else {
        // Gürültü de yok, saf sinyali al
        inputSignal = rawSignal;
    }

    if(inputSignal.isEmpty()) return;




    // UNDO (GERİ AL) İÇİN KAYIT
        // İşlem yapmadan önce elimizdeki sinyali kasaya (yığına) kilitliyoruz.
        // Hafıza şişmesin diye son 10 işlemi tutuyoruz.
    if (undoStack.size() > 10) {
        undoStack.removeFirst(); // En eskiyi sil
    }

    // Paketi (Struct) Oluştur
    UndoState state;
    state.signalData = inputSignal;               // Sinyali koy
    state.sliderValue = ui->sliderFilterParam->value(); // Slider değerini koy    // --------------------------------------------------------
    state.type = UndoFilter;

    // Paketi rafa kaldır (Push)
    undoStack.push(state);


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
    // 1. Önce ses sistemini durdur (Güvenlik için şart)
    stopAudio();

    // 2. Sonra sinyal var mı kontrol et?
    if (rawSignal.isEmpty()) {
        QMessageBox::warning(this, "Hata", "Önce bir sinyal oluşturmalısınız!");
        return;
    }

    // UNDO İÇİN KAYIT ---
    UndoState state;
    state.signalData = noisySignal; // Şu anki gürültülü (veya boş) hali sakla
    state.type = UndoNoise;         // "Bu bir gürültü yedeğidir" de
    undoStack.push(state);

    if (undoStack.size() > 10) undoStack.removeFirst(); // Hafıza dolmasın
    // ----------------------------------------------


    // 3. UI parametrelerini al
    double noiseAmp = ui->dsbNoiseAmplitude->value();
    NoiseType type = static_cast<NoiseType>(ui->cmbNoiseType->currentIndex());

    // 4. Gürültü Ekle (Temiz 'rawSignal'i al, 'noisySignal'e yaz)
    NoiseProcessor::addNoise(rawSignal, noisySignal, type, noiseAmp);

    // 4. Grafiği Güncelle
    // Burada kullanıcı kirli sinyali görsün diye TimePlot'u güncelliyoruz
    m_origTimePlot->updatePlot(timeVec, noisySignal);

    updateFrequencyGraph();
    updateStats(noisySignal);

    // --- 7. ÇIKIŞLARI (FİLTRELİ KISMI) TEMİZLE ---

    // HATA ÇÖZÜMÜ: graph(0)->data()->clear() yerine;
    // updatePlot fonksiyonuna "Boş Vektör" gönderiyoruz. Bu grafiği sıfırlar.
    QVector<double> emptyVec;

    // Alttaki Zaman Grafiğini Temizle
    m_filteredTimePlot->updatePlot(emptyVec, emptyVec);

    // Alttaki FFT Grafiğini Temizle
    m_filteredFreqPlot->updatePlot(emptyVec, emptyVec);

    // Filtrelenmiş sinyal verisini de hafızadan sil
    filteredSignal.clear();

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

    stopAudio();
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

void MainWindow::on_btnBandStop_clicked()
{
    applyAndPlotFilter(FilterType::BAND_STOP);
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

    stopAudio();
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



void MainWindow::on_btnUndo_clicked()
{
    // 1. Yığın Kontrolü
    if (undoStack.isEmpty()) {
        ui->statusbar->showMessage("Geri alınacak işlem yok!", 2000);
        return;
    }

    // 2. Kutuyu (Paketi) Çıkar
    UndoState state = undoStack.pop();


    // --- DURUM A: EĞER GERİ ALINAN ŞEY BİR "GÜRÜLTÜ" İSE ---
    if (state.type == UndoNoise) {

        // Gürültülü sinyali eski haline döndür
        noisySignal = state.signalData;

        // Üst Grafiği (Giriş) Güncelle
        if (noisySignal.isEmpty()) {
            // Eğer gürültü tamamen gittiyse saf sinyali çiz
            m_origTimePlot->updatePlot(timeVec, rawSignal);
        } else {
            // Hala biraz gürültü varsa veya önceki gürültüye döndüysek onu çiz
            m_origTimePlot->updatePlot(timeVec, noisySignal);
        }

        // Üst Frekans Grafiğini Güncelle
        updateFrequencyGraph();

        // İstatistikleri Güncelle (Giriş Sinyali İçin)
        updateStats(noisySignal.isEmpty() ? rawSignal : noisySignal);

        // ALT GRAFİKLERİ TEMİZLE
        // (Çünkü giriş değiştiği için eski filtre sonucu artık geçersizdir)
        filteredSignal.clear();
        QVector<double> empty;
        m_filteredTimePlot->updatePlot(empty, empty);
        m_filteredFreqPlot->updatePlot(empty, empty);

        ui->statusbar->showMessage("Gürültü işlemi geri alındı.", 2000);
    }


    // --- DURUM B: EĞER GERİ ALINAN ŞEY BİR "FİLTRE" İSE ---
    else if (state.type == UndoFilter) {

        // Sinyali Geri Yükle
        filteredSignal = state.signalData;

        // SLIDER'I ESKİ KONUMUNA GETİR
        ui->sliderFilterParam->blockSignals(true); // Sonsuz döngü koruması
        ui->sliderFilterParam->setValue(state.sliderValue);
        ui->sliderFilterParam->blockSignals(false);

        ui->lblSliderValue->setText(QString("Değer: %1").arg(state.sliderValue));

        // Zaman Grafiğini Güncelle
        m_filteredTimePlot->updatePlot(timeVec, filteredSignal);

        // İstatistikleri Güncelle (Çıkış Sinyali İçin)
        updateStats(filteredSignal);

        // FFT Grafiğini Yeniden Hesapla ve Çiz (Senin yazdığın kodun aynısı)
        double fs = ui->txtSampleRate->text().toDouble();
        QVector<double> freqAxis, magVec;
        WindowType wType = static_cast<WindowType>(ui->cmbWindowType->currentIndex());

        FFTProcessor::computeFFT(filteredSignal, fs, freqAxis, magVec, wType);

        bool isDB = (ui->cmbFFTScale->currentIndex() == 1);
        FFTProcessor::applyMagnitudeScaling(magVec, isDB);

        if (isDB)
            m_filteredFreqPlot->setupPlot("Filtre Sonrası Spektrum", "Frekans (Hz)", "Genlik (dB)");
        else
            m_filteredFreqPlot->setupPlot("Filtre Sonrası Spektrum", "Frekans (Hz)", "Genlik");

        m_filteredFreqPlot->updatePlot(freqAxis, magVec);

        ui->statusbar->showMessage("Filtre işlemi geri alındı.", 2000);
    }
}

void MainWindow::playSignal(const QVector<double> &signal, int sampleRate, int type)
{
    if (signal.isEmpty()) return;

    // 1. Eğer zaten bir şey çalıyorsa durdur
    if (audioSink) {
        audioSink->stop();
        delete audioSink;
        audioSink = nullptr;
    }
    if (audioBuffer) {
        audioBuffer->close();
        delete audioBuffer;
        audioBuffer = nullptr;
    }

    // 2. Ses Formatını Ayarla (Standart CD Kalitesi: 16-bit, Mono)
    QAudioFormat format;
    format.setSampleRate(sampleRate);
    format.setChannelCount(1); // Mono
    format.setSampleFormat(QAudioFormat::Int16); // 16-bit ses

    // Cihaz uygun mu kontrol et (Gerekirse varsayılanı al)
    QAudioDevice info = QMediaDevices::defaultAudioOutput();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Varsayılan format desteklenmiyor, en yakın formatı deniyorum.";
        format = info.preferredFormat();
    }

    // 3. Veriyi Dönüştür (Double -> Int16 -> Byte)
    audioBytes.clear();
    QDataStream stream(&audioBytes, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian); // WAV standardı

    int counter = 0;

    for (double sample : signal) {
        // Clipping Koruması: Sinyal 1.0'ı geçerse cızırtı yapar, sınırlayalım.
        if (sample > 1.0) sample = 1.0;
        if (sample < -1.0) sample = -1.0;

        // Double (-1.0 .. 1.0) -> Int16 (-32768 .. 32767)
        qint16 pcmSample = static_cast<qint16>(sample * 32767.0);
        stream << pcmSample;

        // --- DONMAYI ENGELLEYEN KOD ---
        // Her 10.000 örnekte bir arayüze "Nefes al, tıklamaları kontrol et" diyoruz.
        counter++;
        if (counter % 10000 == 0) {
            QCoreApplication::processEvents();
        }
        // --------------------------------------
    }

    // 4. Oynatıcıyı Hazırla
    audioBuffer = new QBuffer(&audioBytes);
    audioBuffer->open(QIODevice::ReadOnly); // Okuma modunda aç

    audioSink = new QAudioSink(info, format, this);


    // SES SEVİYESİNİ SLIDER'DAN AL ---
    // Slider'ın o anki değerini okuyup uygula
    qreal currentVolume = ui->sliderVolume->value() / 100.0;
    audioSink->setVolume(currentVolume);
    // --------------------------------------------------------

    // Sesi çalmaya başla!
    connect(audioSink, &QAudioSink::stateChanged, this, [&](QAudio::State state){
        if (state == QAudio::IdleState) {
            // Ses bitince buffer'ı kapatabiliriz veya durdur butonu halleder.
            // Burası ses bitince otomatik bir şey yapmak istersen (örn. Loop) kullanılır.
        }
    });


    // TİPİ KAYDET ---
    currentAudioType = type;
    // ---------------------------------


    // Sesi çalmaya başla!
    audioSink->start(audioBuffer);

    ui->statusbar->showMessage("Ses çalınıyor...", 3000);
}

void MainWindow::on_btnPlayInput_clicked()
{
    // 1. Durum: Zaten bu sinyal (Giriş) hafızadaysa
    if (audioSink && currentAudioType == 1) {

        // Çalıyorsa -> DURAKLAT
        if (audioSink->state() == QAudio::ActiveState) {
            audioSink->suspend();
            // İKONU DEĞİŞTİR: "Play" Yap (Çünkü durdu, basınca çalmalı)

            ui->btnPlayInput->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            ui->btnPlayInput->setText(""); // Yazıyı sil ki sadece ikon görünsün (isteğe bağlı)

            ui->statusbar->showMessage("Giriş sinyali duraklatıldı.", 2000);
            return; // Çık, yeniden başlatma
        }

        // Durakladıysa -> DEVAM ET
        else if (audioSink->state() == QAudio::SuspendedState) {
            audioSink->resume();

            // İKONU DEĞİŞTİR: "Pause" Yap (Çünkü çalıyor, basınca durmalı)
            ui->btnPlayInput->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

            ui->statusbar->showMessage("Devam ediliyor...", 2000);
            return; // Çık
        }
    }


    // Giriş sinyalini çal (noisy varsa onu, yoksa raw'ı)
    QVector<double> signalToPlay = noisySignal.isEmpty() ? rawSignal : noisySignal;

    // Sample Rate kutusundan değeri al
    int fs = ui->txtSampleRate->text().toInt();

    // Tip olarak '1' (Giriş) gönderiyoruz
    playSignal(signalToPlay, fs, 1);

    // Buton yazısını güncelle
    ui->btnPlayInput->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    //playSignal(signalToPlay, fs);
}


void MainWindow::on_btnPlayOutput_clicked()
{
    if (filteredSignal.isEmpty()) {
        ui->statusbar->showMessage("Çalınacak filtrelenmiş sinyal yok!", 2000);
        return;
    }

    // 1. Durum: Zaten bu sinyal (Çıkış) hafızadaysa
    if (audioSink && currentAudioType == 2) {

        if (audioSink->state() == QAudio::ActiveState) {
            audioSink->suspend();


            // İKONU DEĞİŞTİR: "Play" Yap (Çünkü durdu, basınca çalmalı)
            ui->btnPlayOutput->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            ui->btnPlayOutput->setText(""); // Yazıyı sil ki sadece ikon görünsün (isteğe bağlı)
            ui->statusbar->showMessage("Çıkış sinyali duraklatıldı.", 2000);
            return;
        }
        else if (audioSink->state() == QAudio::SuspendedState) {
            audioSink->resume();
            // İKONU DEĞİŞTİR: "Pause" Yap (Çünkü çalıyor, basınca durmalı)
            ui->btnPlayOutput->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            ui->statusbar->showMessage("Devam ediliyor...", 2000);
            return;
        }
    }

    // 2. Durum: Sıfırdan Başlat
    int fs = ui->txtSampleRate->text().toInt();

    // Tip olarak '2' (Çıkış) gönderiyoruz
    playSignal(filteredSignal, fs, 2);

    // Başladığı an butonu "Pause" ikonuna çevir
    ui->btnPlayOutput->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}

void MainWindow::on_sliderVolume_valueChanged(int value)
{
    // Slider 0-100 arası geliyor, biz onu 0.0 - 1.0 arasına çeviriyoruz.
    qreal linearVolume = value / 100.0;

    // Eğer o an ses çıkış nesnesi açıksa sesini güncelle
    if (audioSink) {
        audioSink->setVolume(linearVolume);
    }

    // Kullanıcıya bilgi ver (Opsiyonel)
    ui->statusbar->showMessage(QString("Ses Seviyesi: %%1").arg(value), 1000);
}

void MainWindow::stopAudio()
{
    // 1. Ses Çıkış Nesnesi Varsa Durdur
    if (audioSink) {
        audioSink->stop();
        audioSink->reset(); // Tamamen sıfırla
    }

    // 2. Buffer'ı Kapat
    if (audioBuffer) {
        audioBuffer->close();
    }

    // 3. Durumu Sıfırla
    currentAudioType = 0; // Hiçbir şey çalmıyor

    // 4. UI Güncelle (Play/Pause butonları varsa sıfırla)
    // stopAudio() fonksiyonunun içine:
    if (ui->btnPlayInput) ui->btnPlayInput->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    if (ui->btnPlayOutput) ui->btnPlayOutput->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}
