#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filterprocessor.h"
#include <QMessageBox>
#include <QFileDialog>
#include "utils.h"
#include <QInputDialog>
#include "statsprocessor.h"
#include <QStyle>
#include <QTimer>
#include "qcustomplot.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // -------------------------------------------------------------------------
    // ADIM 1: ÖNCE GRAFİK YÖNETİCİLERİNİ OLUŞTUR (ÇOK ÖNEMLİ!)
    // -------------------------------------------------------------------------

    // PlotManager'ı oluşturup, ona UI'daki "customPlotTimeOriginal"i teslim ediyoruz.
    m_origTimePlot = new PlotManager(ui->customPlotTimeOriginal);
    m_origTimePlot->setupPlot("Orijinal Sinyal", "Zaman (s)", "Genlik");

    // PlotManager'ı oluşturup, ona UI'daki "customPlotFreqOriginal"i teslim ediyoruz.
    m_origFreqPlot = new PlotManager(ui->customPlotFreqOriginal);
    m_origFreqPlot->setupPlot("Frekans Spektrumu", "Frekans (Hz)", "Genlik");

    // PlotManager'ı oluşturup, ona UI'daki "customPlotTimeFiltered"i teslim ediyoruz.
    m_filteredTimePlot = new PlotManager(ui->customPlotTimeFiltered);
    m_filteredTimePlot->setupPlot("Filtrelenmiş Sinyal", "Zaman (s)", "Genlik");

    // PlotManager'ı oluşturup, ona UI'daki "customPlotFreqFiltered"i teslim ediyoruz.
    m_filteredFreqPlot = new PlotManager(ui->customPlotFreqFiltered);
    m_filteredFreqPlot->setupPlot("Filtre Sonrası Spektrum", "Frekans (Hz)", "Genlik");


    // -------------------------------------------------------------------------
    // ADIM 2: KIRMIZI ÇİZGİYİ (PLAYHEAD) OLUŞTUR
    // -------------------------------------------------------------------------

    // 1. Kırmızı Çizgiyi Oluştur (Giriş Grafiği İçin)
    playHeadLine = new QCPItemLine(m_origTimePlot->getPlot());
    playHeadLine->setPen(QPen(Qt::red, 2));
    playHeadLine->start->setCoords(0, -2);
    playHeadLine->end->setCoords(0, 2);
    playHeadLine->setVisible(false);

    // 2. Çıkış Çizgisi (Filtreli Grafik İçin)
    playHeadLineOutput = new QCPItemLine(m_filteredTimePlot->getPlot());
    playHeadLineOutput->setPen(QPen(Qt::red, 2));
    playHeadLineOutput->start->setCoords(0, -2);
    playHeadLineOutput->end->setCoords(0, 2);
    playHeadLineOutput->setVisible(false);


    // -------------------------------------------------------------------------
    // ADIM 3: TIMER (ZAMANLAYICI) KURULUMU
    // -------------------------------------------------------------------------
    playHeadTimer = new QTimer(this);
    playHeadTimer->setInterval(50); // 50ms (20 FPS)
    connect(playHeadTimer, &QTimer::timeout, this, &MainWindow::updatePlayHead);


    // -------------------------------------------------------------------------
    // ADIM 4: ARAYÜZ ELEMANLARINI DOLDUR
    // -------------------------------------------------------------------------

    // ComboBox Doldurma
    ui->cmbSignalType->addItems({"Sinüs", "Kare", "Üçgen", "Testere Dişi"});
    ui->cmbNoiseType->addItems({"White Noise", "Impulse Noise", "Sinusoidal Noise"});
    ui->cmbWindowType->addItems({"Rectangular", "Hann", "Hamming", "Blackman"});
    ui->cmbFFTScale->addItems({"Lineer", "dB (Logaritmik)"});

    // Slider Başlangıç Ayarları
    ui->sliderFilterParam->setRange(1, 50);
    ui->sliderFilterParam->setValue(5);
    ui->lblSliderValue->setText("Değer: 5");

    // Varsayılan Filtre Türü
    currentFilterType = FilterType::MOVING_AVERAGE;

    // RealTimeHandler Sınıfını Oluştur
    m_realTimeHandler = new RealTimeHandler(this);

    // Bağlantılar
    connect(m_realTimeHandler, &RealTimeHandler::dataReady,
            this, &MainWindow::updateLivePlot);

    connect(ui->cmbFFTScale, &QComboBox::currentIndexChanged, this, &MainWindow::updateAllGraphs);


    // -------------------------------------------------------------------------
    // ADIM 5: GÖRSEL MAKYAJ (GRAFİKLERİ GÜVENLİ KARARTMA)
    // -------------------------------------------------------------------------
    // Bu kısım constructor'ın en sonunda olmalı ki grafikler oluşmuş olsun.

    // İşlenecek grafiklerin listesini oluştur
    QList<QCustomPlot*> plots;
    if(m_origTimePlot && m_origTimePlot->getPlot()) plots << m_origTimePlot->getPlot();
    if(m_origFreqPlot && m_origFreqPlot->getPlot()) plots << m_origFreqPlot->getPlot();
    if(m_filteredTimePlot && m_filteredTimePlot->getPlot()) plots << m_filteredTimePlot->getPlot();
    if(m_filteredFreqPlot && m_filteredFreqPlot->getPlot()) plots << m_filteredFreqPlot->getPlot();

    for(QCustomPlot* plot : plots) {
        if(!plot) continue; // Ekstra güvenlik

        // 1. Arka Planı Koyu Lacivert Yap (CSS ile uyumlu: #111721)
        plot->setBackground(QBrush(QColor(17, 23, 33)));

        // 2. Eksen Çizgilerini Beyaz Yap
        plot->xAxis->setBasePen(QPen(Qt::white));
        plot->yAxis->setBasePen(QPen(Qt::white));
        plot->xAxis->setTickPen(QPen(Qt::white));
        plot->yAxis->setTickPen(QPen(Qt::white));
        plot->xAxis->setSubTickPen(QPen(Qt::white));
        plot->yAxis->setSubTickPen(QPen(Qt::white));

        // 3. Yazıları Beyaz Yap
        plot->xAxis->setTickLabelColor(Qt::white);
        plot->yAxis->setTickLabelColor(Qt::white);
        plot->xAxis->setLabelColor(Qt::white);
        plot->yAxis->setLabelColor(Qt::white);

        // 4. Izgaraları (Grid) Silikleştir
        plot->xAxis->grid()->setPen(QPen(QColor(60, 70, 80), 1, Qt::SolidLine));
        plot->yAxis->grid()->setPen(QPen(QColor(60, 70, 80), 1, Qt::SolidLine));
        plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
        plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);

        // 5. Değişiklikleri Uygula
        plot->replot();
    }

    // -------------------------------------------------------------------------
    // ADIM 6: SİNYAL RENKLERİNİ AYARLA (NEON MAVİ & ADAÇAYI YEŞİLİ)
    // -------------------------------------------------------------------------

    // 1. GİRİŞ GRAFİKLERİ -> NEON CYAN (Mavi)
    QColor inputColor(0, 210, 255);

    if(m_origTimePlot && m_origTimePlot->getPlot()->graphCount() > 0)
        m_origTimePlot->getPlot()->graph(0)->setPen(QPen(inputColor, 2));

    if(m_origFreqPlot && m_origFreqPlot->getPlot()->graphCount() > 0)
        m_origFreqPlot->getPlot()->graph(0)->setPen(QPen(inputColor, 2));


    // 2. ÇIKIŞ GRAFİKLERİ -> ADAÇAYI YEŞİLİ (#459363)
    QColor outputColor("#e31212");

    if(m_filteredTimePlot && m_filteredTimePlot->getPlot()->graphCount() > 0)
        m_filteredTimePlot->getPlot()->graph(0)->setPen(QPen(outputColor, 2));

    if(m_filteredFreqPlot && m_filteredFreqPlot->getPlot()->graphCount() > 0)
        m_filteredFreqPlot->getPlot()->graph(0)->setPen(QPen(outputColor, 2));

    // Son kez replot yapıp renkleri oturtalım
    if(m_origTimePlot) m_origTimePlot->getPlot()->replot();
    if(m_filteredTimePlot) m_filteredTimePlot->getPlot()->replot();
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
    // Hangi sinyali çizeceğiz? (Gürültülü varsa onu, yoksa saf sinyali)
    QVector<double> signalToPlot = noisySignal.isEmpty() ? rawSignal : noisySignal;

    if (signalToPlot.isEmpty()) return;

    // Parametreleri Arayüzden Al
    double fs = ui->txtSampleRate->text().toDouble();
    bool isDB = (ui->cmbFFTScale->currentIndex() == 1);

    // --- İŞTE EKSİK OLAN SATIR BUYDU ---
    // ComboBox'tan seçilen pencere tipini okuyoruz
    WindowType wType = static_cast<WindowType>(ui->cmbWindowType->currentIndex());
    // -----------------------------------

    // FFT Hesapla (wType parametresini buraya veriyoruz!)
    QVector<double> freqAxis, magVec;
    FFTProcessor::computeFFT(signalToPlot, fs, freqAxis, magVec, wType);

    // dB Dönüşümü
    FFTProcessor::applyMagnitudeScaling(magVec, isDB);

    // Başlık ve Eksenler
    if (isDB)
        m_origFreqPlot->setupPlot("Frekans Spektrumu (Giriş)", "Frekans (Hz)", "Genlik (dB)");
    else
        m_origFreqPlot->setupPlot("Frekans Spektrumu (Giriş)", "Frekans (Hz)", "Genlik");

    // Çiz
    m_origFreqPlot->updatePlot(freqAxis, magVec);
}

void MainWindow::applyAndPlotFilter(FilterType type)
{
    // 1. GÜVENLİK KONTROLLERİ
    if (!m_filteredTimePlot || !m_filteredFreqPlot) return;
    if (rawSignal.isEmpty()) return;

    // 2. UI GÜNCELLEMELERİ
    currentFilterType = type;
    double param = ui->sliderFilterParam->value();

    QString paramName = "Değer";
    if (type == FilterType::MOVING_AVERAGE || type == FilterType::MEDIAN) {
        paramName = "Pencere Boyutu (N)";
    } else if (type == FilterType::LOW_PASS || type == FilterType::HIGH_PASS) {
        paramName = "Kesim Frekansı (Hz)";
    } else if (type == FilterType::BAND_STOP || type == FilterType::BAND_PASS) {
        paramName = "Merkez Frekans (Hz)";
    } else if (type == FilterType::KALMAN) {  // <--- BURAYA EKLİYORUZ
        paramName = "Ölçüm Gürültüsü (R)";
    }

    ui->lblSliderValue->setText(QString("%1: %2").arg(paramName).arg(param));


    // 3. GİRDİ SİNYALİNİ SEÇ (Input Selection)
    // Mantık: Elimizde gürültülü/işlenmiş bir sinyal varsa onu filtrele,
    // yoksa saf sinyali filtrele.

    QVector<double> inputSignal;
    if (!noisySignal.isEmpty()) {
        inputSignal = noisySignal;
    } else {
        inputSignal = rawSignal;
    }

    if(inputSignal.isEmpty()) return;


    // 4. UNDO (GERİ AL) KAYDI
    if (undoStack.size() > 10) {
        undoStack.removeFirst();
    }

    UndoState state;
    state.signalData = inputSignal;         // Şu anki halini sakla
    state.sliderValue = ui->sliderFilterParam->value();
    state.type = UndoFilter;
    undoStack.push(state);


    // 5. ÖRNEKLEME FREKANSINI HESAPLA (MİKROFON UYUMLU)
    // Kutudaki yazıya güvenme, gerçek veriye bak!
    double duration = (!timeVec.isEmpty()) ? timeVec.last() : 0.0;
    double fs = (duration > 0) ? (rawSignal.size() / duration) : 48000.0;


    // 6. FİLTREYİ HESAPLA
    // SONUCU 'noisySignal' İÇİNE YAZIYORUZ!
    // Böylece Play butonu ve Grafikler güncel veriyi görüyor.

    FilterProcessor::applyFilter(inputSignal, noisySignal, type, fs, param);


    // 7. HER ŞEYİ GÜNCELLE (Sihirli Dokunuş)
    // Zaman grafiği, FFT grafiği ve İstatistikler tek seferde güncellenir.
    updateAllGraphs();

    ui->statusbar->showMessage("Filtre uygulandı.");
}



void MainWindow::on_btnAddSignal_clicked()
{
    // 1. UI'dan Verileri Al (Değişmedi)
    if(ui->txtSampleRate->text().isEmpty() || ui->txtDuration->text().isEmpty() /* ...diğer kontroller... */) return;

    double fs = ui->txtSampleRate->text().toDouble();
    double duration = ui->txtDuration->text().toDouble();
    double freq = ui->txtSignalFreq->text().toDouble();
    double amplitude = ui->dsbSignalAmplitude->value();
    SignalType type = static_cast<SignalType>(ui->cmbSignalType->currentIndex());

    // 2. Sinyali Üret (Değişmedi)
    SignalGenerator::generateSignal(type, fs, duration, freq, amplitude, timeVec, rawSignal);

    // 3. KRİTİK NOKTA: İşlenecek sinyali (noisy) de sıfırla
    // Yeni bir sinüs ürettik, o yüzden eski gürültülü sinyali unut, temizle başla.
    noisySignal = rawSignal;

    // 4. MİKROFON MODUNU KAPAT (Çakışma olmasın)
    isRecording = false; // Eğer kayıt değişkenin varsa
    // ui->chkRealTime->setChecked(false); // Eğer checkbox kullanıyorsan

    // 5. HER ŞEYİ ÇİZDİR
    updateAllGraphs();

    ui->statusbar->showMessage("Sinyal üretildi.", 2000);
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


    if (undoStack.size() > 10) undoStack.removeFirst(); // Hafıza dolmasın
    // ----------------------------------------------

    // UNDO İÇİN KAYIT ---
    UndoState state;
    // Eğer noisySignal boşsa (ilk kez gürültü ekleniyorsa), temiz halini (rawSignal) yedekle
    // Eğer doluysa (zaten gürültülü veya filtreliyse), o halini yedekle
    state.signalData = noisySignal.isEmpty() ? rawSignal : noisySignal;
    state.type = UndoNoise;         // "Bu bir gürültü yedeğidir" de
    undoStack.push(state);


    // 3. UI parametrelerini al
    double noiseAmp = ui->dsbNoiseAmplitude->value();
    NoiseType type = static_cast<NoiseType>(ui->cmbNoiseType->currentIndex());

    // GÜRÜLTÜYÜ HESAPLA (Temiz sinyale ekle)
    // DİKKAT: Her zaman temiz sinyalin (rawSignal) üzerine ekliyoruz ki
    // gürültüler üst üste binip bozulmasın.
    // "rawSignal"den al -> "noisySignal"e yaz.

    // 4. Gürültü Ekle (Temiz 'rawSignal'i al, 'noisySignal'e yaz)
    NoiseProcessor::addNoise(rawSignal, noisySignal, type, noiseAmp);

    // 5. ESKİ FİLTRE SONUÇLARINI TEMİZLE (Opsiyonel ama önerilir)
    // Yeni gürültü ekleyince eski filtreli verinin anlamı kalmaz.
    // filteredSignal.clear(); // Eğer bu değişkeni hala kullanıyorsan açabilirsin.


    // 6. GRAFİKLERİ GÜNCELLE (SİHİRLİ NOKTA)
    // Bu fonksiyon:
    // - Üst grafiğe rawSignal (Temiz) çizer.
    // - Alt grafiğe noisySignal (Gürültülü) çizer.
    // - Eksenleri ayarlar.

    updateAllGraphs();

    ui->statusbar->showMessage("Gürültü eklendi (Alt grafiğe bakınız).");

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
    // 1. Üst Grafiği (Giriş) Güncelle
    // (Zaten senin var olan fonksiyonun)
    updateFrequencyGraph();

    // 2. Alttaki Grafiği (Çıkış) Güncelle
    updateOutputFFT();
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

void MainWindow::on_btnKalman_clicked()
{
    // Ortak fonksiyonumuzu kullanıyoruz
    applyAndPlotFilter(FilterType::KALMAN);

    // Kullanıcıya bilgi verelim
    ui->statusbar->showMessage("Kalman Filtresi Aktif! Slider ile 'R' (Güven) katsayısını ayarlayabilirsiniz.");
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
    // EĞER MÜZİK ÇALIYORSA ÖNCE DURDUR (Senkronizasyon bozulmasın)
    stopAudio();

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

        updateOutputFFT();

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



    playHeadLine->setVisible(true); // Çizgiyi göster
    playHeadTimer->start();         // Hareketi başlat


    // Sesi çalmaya başla!
    audioSink->start(audioBuffer);

    ui->statusbar->showMessage("Ses çalınıyor...", 3000);
}

void MainWindow::on_btnPlayInput_clicked()
{
    // 1. GÜVENLİK: Sinyal yoksa işlem yapma
    if (rawSignal.isEmpty()) {
        ui->statusbar->showMessage("Çalınacak giriş sinyali yok!", 2000);
        return;
    }

    // 1. Durum: Zaten bu sinyal (Giriş) hafızadaysa
    if (audioSink && currentAudioType == 1) {

        // Çalıyorsa -> DURAKLAT
        if (audioSink->state() == QAudio::ActiveState) {
            audioSink->suspend();
            // İKONU DEĞİŞTİR: "Play" Yap (Çünkü durdu, basınca çalmalı)

            ui->btnPlayInput->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            ui->btnPlayInput->setText(""); // Yazıyı sil ki sadece ikon görünsün (isteğe bağlı)

            playHeadTimer->stop(); // Çizgiyi dondur
            ui->statusbar->showMessage("Giriş sinyali duraklatıldı.", 2000);
            return; // Çık, yeniden başlatma
        }

        // Durakladıysa -> DEVAM ET
        else if (audioSink->state() == QAudio::SuspendedState) {
            audioSink->resume();

            // İKONU DEĞİŞTİR: "Pause" Yap (Çünkü çalıyor, basınca durmalı)
            ui->btnPlayInput->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

            playHeadTimer->start(); // Çizgiyi tekrar hareket ettir
            ui->statusbar->showMessage("Devam ediliyor...", 2000);
            return; // Çık
        }
    }


    // 3. FREKANS HESABI (Akıllı Yöntem)
    int fs = 48000;
    if (!timeVec.isEmpty() && timeVec.last() > 0) {
        fs = (int)(rawSignal.size() / timeVec.last());
    } else if (!ui->txtSampleRate->text().isEmpty()) {
        fs = ui->txtSampleRate->text().toInt();
        if (fs <= 0) fs = 48000;
    }

    // Eskiden kutudan alıyorduk (fs değişkeni), şimdi mikrofon hızını zorluyoruz:
    // Sample Rate kutusundan değeri al
    //int fs = ui->txtSampleRate->text().toInt();

    // Doğrudan 48000 gönderiyoruz:
    playSignal(rawSignal, 48000, 1);

    // Buton yazısını güncelle
    ui->btnPlayInput->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->statusbar->showMessage("Giriş (Saf) sinyali çalınıyor...", 2000);
    //playSignal(signalToPlay, fs);
}


void MainWindow::on_btnPlayOutput_clicked()
{
    // 1. OYNATILACAK SİNYALİ BELİRLE
    // Biz artık çıkış verisini 'noisySignal' içine kaydediyoruz.
    // Eğer noisySignal boşsa, belki eski 'filteredSignal' doludur diye ona bak.
    QVector<double> signalToPlay;

    if (!noisySignal.isEmpty()) {
        signalToPlay = noisySignal;
    } else if (!filteredSignal.isEmpty()) {
        signalToPlay = filteredSignal;
    }

    if (signalToPlay.isEmpty()) {
        ui->statusbar->showMessage("Çalınacak filtrelenmiş sinyal yok! Önce bir filtre uygulayın.", 2000);
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

    // 3. FREKANS AYARI (SİNYAL ÜRETECİ Mİ? MİKROFON MU?)
    int fs = 48000; // Varsayılan (Mikrofon)

    // Eğer zaman vektörümüz varsa en doğrusu oradan hesaplamaktır
    if (!timeVec.isEmpty() && timeVec.last() > 0) {
        fs = (int)(rawSignal.size() / timeVec.last());
    }
    // Yoksa ve kutu doluysa kutuyu kullan
    else if (!ui->txtSampleRate->text().isEmpty()) {
        int val = ui->txtSampleRate->text().toInt();
        if (val > 0) fs = val;
    }

    // Tip olarak '2' (Çıkış) gönderiyoruz
    playSignal(signalToPlay, fs, 2);

    // Başladığı an butonu "Pause" ikonuna çevir
    ui->btnPlayOutput->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->statusbar->showMessage("Filtrelenmiş sinyal çalınıyor...", 2000);
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
    // 1. Önce Ses Motorunu Durdur ve Sıfırla
    if (audioSink) {
        // Sadece çalışıyor veya duraklatılmışsa stop de
        if (audioSink->state() == QAudio::ActiveState || audioSink->state() == QAudio::SuspendedState) {
            audioSink->stop();
        }
        audioSink->reset(); // Buffer ile bağlantıyı kopar (Kritik nokta)
    }

    // 2. Playhead (Kırmızı Çizgi) ve Timer Temizliği
    if (playHeadTimer && playHeadTimer->isActive()) {
        playHeadTimer->stop();
    }

    if (playHeadLine) {
        playHeadLine->setVisible(false);
        playHeadLine->start->setCoords(0, -2); // Başlangıç konumu
        playHeadLine->end->setCoords(0, 2);
    }

    // Çıkış Çizgisini Sıfırla
    if (playHeadLineOutput) {
        playHeadLineOutput->setVisible(false);
        playHeadLineOutput->start->setCoords(0, -2);
        playHeadLineOutput->end->setCoords(0, 2);
    }

    // Grafik çizimini güvenli hale getir
    if (m_origTimePlot) {
        m_origTimePlot->getPlot()->replot();
    }

    // 3. Buffer'ı Kapat (Ses motoru ayrıldıktan sonra)
    if (audioBuffer && audioBuffer->isOpen()) {
        audioBuffer->close();
    }

    // 4. Durumu ve Arayüzü Sıfırla
    currentAudioType = 0;

    // Buton ikonlarını "Play" moduna getir
    if (ui->btnPlayInput) ui->btnPlayInput->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    if (ui->btnPlayOutput) ui->btnPlayOutput->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    if (ui->statusbar) ui->statusbar->showMessage("Ses durduruldu.", 2000);
}

void MainWindow::updatePlayHead()
{
    if (!audioSink) return;

    // Şu an kaçıncı mikrosaniyedeyiz? (1 sn = 1.000.000 mikrosaniye)
    qint64 processedUSecs = audioSink->processedUSecs();

    // Saniyeye çevir (X ekseni saniye olduğu için)
    double currentSec = processedUSecs / 1000000.0;

    // 2. HANGİSİ ÇALIYORSA ONU GÜNCELLE
    if (currentAudioType == 1) {
        // --- GİRİŞ SİNYALİ ÇALIYOR ---

        // Giriş çizgisini güncelle ve göster
        playHeadLine->start->setCoords(currentSec, -10);
        playHeadLine->end->setCoords(currentSec, 10);
        playHeadLine->setVisible(true);

        // Çıkış çizgisini gizle
        playHeadLineOutput->setVisible(false);

        // Sadece giriş grafiğini yenile
        m_origTimePlot->getPlot()->replot();
    }
    else if (currentAudioType == 2) {
        // --- ÇIKIŞ (FİLTRELİ) SİNYALİ ÇALIYOR ---

        // Çıkış çizgisini güncelle ve göster
        playHeadLineOutput->start->setCoords(currentSec, -10);
        playHeadLineOutput->end->setCoords(currentSec, 10);
        playHeadLineOutput->setVisible(true);

        // Giriş çizgisini gizle
        playHeadLine->setVisible(false);

        // Sadece çıkış grafiğini yenile
        m_filteredTimePlot->getPlot()->replot();
    }

}

void MainWindow::updateOutputFFT()
{
    // 1. Eğer filtrelenmiş sinyal yoksa grafiği temizle ve çık
    if (filteredSignal.isEmpty()) {
        QVector<double> empty;
        m_filteredFreqPlot->updatePlot(empty, empty);
        return;
    }

    // 2. Gerekli parametreleri al
    double fs = ui->txtSampleRate->text().toDouble();
    WindowType wType = static_cast<WindowType>(ui->cmbWindowType->currentIndex());
    bool isDB = (ui->cmbFFTScale->currentIndex() == 1); // 0: Lineer, 1: dB

    // 3. FFT Hesapla
    QVector<double> freqAxis, magVec;
    FFTProcessor::computeFFT(filteredSignal, fs, freqAxis, magVec, wType);

    // 4. dB Dönüşümü Yap (Eğer seçiliyse)
    FFTProcessor::applyMagnitudeScaling(magVec, isDB);

    // 5. Başlıkları Ayarla
    if (isDB)
        m_filteredFreqPlot->setupPlot("Filtre Sonrası Spektrum", "Frekans (Hz)", "Genlik (dB)");
    else
        m_filteredFreqPlot->setupPlot("Filtre Sonrası Spektrum", "Frekans (Hz)", "Genlik");

    // 6. Çiz
    m_filteredFreqPlot->updatePlot(freqAxis, magVec);
}

void MainWindow::on_cmbWindowType_currentIndexChanged(int index)
{
    // 1. Üstteki (Giriş) Frekans Grafiğini Güncelle
    updateFrequencyGraph();

    // 2. Alttaki (Filtreli) Frekans Grafiğini Güncelle
    updateOutputFFT();
}

void MainWindow::on_chkRealTime_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        m_origTimePlot->clearPlot(); // Grafiği temizle
        m_liveBuffer.clear();        // Tamponu boşalt
        m_realTimeHandler->startMicrophone(); // Mikrofonu aç
        ui->statusbar->showMessage("Mikrofon Dinleniyor...");
    } else {
        m_realTimeHandler->stopMicrophone(); // Mikrofonu kapat
        ui->statusbar->showMessage("Canlı mod durduruldu.");
    }
}

void MainWindow::updateLivePlot(const QVector<double> &newData)
{
    // Sadece kayıt modundaysak çiz
    if (!isRecording) return;

    // Gelen veriyi ekle
    m_liveBuffer.append(newData);

    // Son 5 saniyeyi tut (44100 * 5 = 220500 örnek)
    if (m_liveBuffer.size() > 220500) {
        m_liveBuffer.remove(0, m_liveBuffer.size() - 220500);
    }

    // Zaman ekseni oluştur (X ekseni)
    QVector<double> timeAxis(m_liveBuffer.size());
    for(int i=0; i < m_liveBuffer.size(); ++i) {
        timeAxis[i] = i / 48000.0;
    }

    // Grafiği çizdir
    m_origTimePlot->updatePlot(timeAxis, m_liveBuffer);

    // Grafiği kaydır (Auto-Scroll)
    if (!timeAxis.isEmpty()) {
        m_origTimePlot->getPlot()->xAxis->setRange(timeAxis.last() - 5.0, timeAxis.last());
        m_origTimePlot->getPlot()->replot();
    }
}

/*
void MainWindow::on_btnRecord_clicked()
{
    // Durum kontrolü: Şu an kayıt yapıyor muyuz?
    if (!isRecording) {
        // --- KAYDI BAŞLAT ---

        isRecording = true;
        ui->btnRecord->setText("⏹️ Durdur"); // Buton yazısını değiştir
        ui->btnRecord->setStyleSheet("; color: white; border: 1px solid red;");
        ui->statusbar->showMessage("Kayıt yapılıyor... Konuşabilirsiniz.");

        // 1. Önceki verileri temizle
        m_origTimePlot->clearPlot();
        m_liveBuffer.clear(); // Canlı tamponu boşalt

        // 2. Diğer girişleri kilitle (Kayıt sırasında oynama yapılmasın)
        ui->groupBox_2->setEnabled(false);
        ui->btnPlayInput->setEnabled(false);

        // 3. Mikrofonu Başlat (Handler sınıfını kullanıyoruz)
        m_realTimeHandler->startMicrophone();
    }
    else {
        // --- KAYDI BİTİR VE İŞLE ---

        isRecording = false;
        ui->btnRecord->setText("🔴 Kayıt"); // Yazıyı eski haline getir

        ui->btnRecord->setStyleSheet(""); // Rengi sıfırla

        // 1. Mikrofonu Durdur
        m_realTimeHandler->stopMicrophone();

        // 2. KRİTİK NOKTA: Canlı veriyi ana işlem vektörlerine aktar!
        if (m_liveBuffer.isEmpty()) {
            ui->statusbar->showMessage("Ses kaydedilemedi!");
            return;
        }

        // Artık 'rawSignal' bizim kaydettiğimiz sestir.
        rawSignal = m_liveBuffer;
        noisySignal = m_liveBuffer; // Gürültülü sinyal de başlangıçta aynıdır

        // 3. Zaman vektörünü oluştur (X ekseni için)
        // time = index / SampleRate
        timeVec.resize(rawSignal.size());
        for(int i=0; i < rawSignal.size(); ++i) {
            timeVec[i] = (double)i / 48000.0;
        }

        // 4. BÜTÜN GRAFİKLERİ GÜNCELLE
        // Artık kayıt bittiği için FFT (Frekans) grafiğini de çizdirebiliriz!

        // Zaman Grafiği (Tamamını göster)
        m_origTimePlot->updatePlot(timeVec, rawSignal);
        m_origTimePlot->getPlot()->xAxis->setRange(0, timeVec.last()); // Zoom out yap
        m_origTimePlot->getPlot()->replot();

        // Filtreli Grafik (Başlangıçta orijinalle aynı)
        m_filteredTimePlot->updatePlot(timeVec, noisySignal);

        // Frekans Analizini Yap (FFT Fonksiyonunu tetikle)
        // Not: updateFFT fonksiyonun varsa onu çağır. Yoksa buraya FFT kodunu ekleriz.
        // updateFFTPlots();

        // İstatistikleri Hesapla
        updateStats(noisySignal);

        // 5. Kilitleri Aç
        ui->groupBox_2->setEnabled(true);
        ui->btnPlayInput->setEnabled(true);
        ui->statusbar->showMessage("Kayıt tamamlandı. İşlem yapabilirsiniz.");
    }
} */

void MainWindow::on_btnRecord_clicked()
{
    if (!isRecording) {
        // --- KAYIT BAŞLA ---
        isRecording = true;
        ui->btnRecord->setText("⏹️ Durdur");
        ui->btnRecord->setStyleSheet("background-color: #ff3333; color: white; border: 1px solid red;");

        m_liveBuffer.clear();
        m_realTimeHandler->startMicrophone();

        // Diğer butonları kilitle
        ui->btnPlayInput->setEnabled(false);
        ui->btnPlayOutput->setEnabled(false); // Çıkış oynatmayı da kilitle
    }
    else {
        // --- KAYIT BİTİR ---
        isRecording = false;
        ui->btnRecord->setText("🔴 Kayıt");
        ui->btnRecord->setStyleSheet("");
        m_realTimeHandler->stopMicrophone();

        if (m_liveBuffer.isEmpty()) return;

        // 1. SADECE RAW SİNYALİ DOLDUR
        rawSignal = m_liveBuffer;

        // 2. İŞLENMİŞ SİNYALİ TEMİZLE (Çünkü daha filtrelemedik)
        noisySignal.clear();

        // 3. GRAFİKLERİ GÜNCELLE
        updateAllGraphs(); // Bu artık sadece üstü çizecek, altı temizleyecek.

        ui->btnPlayInput->setEnabled(true);
        ui->btnPlayOutput->setEnabled(true);
        ui->statusbar->showMessage("Kayıt tamamlandı. Filtre uygulamak için butonları kullanın.");
    }
}


void MainWindow::updateAllGraphs()
{
    // Veri yoksa çık
    if (rawSignal.isEmpty()) return;

    // 1. ZAMAN VEKTÖRÜNÜ GÜNCELLE
    // Eğer zaman vektörü eksikse veya boyutu tutmuyorsa yeniden oluştur
    if (timeVec.size() != rawSignal.size()) {
        timeVec.resize(rawSignal.size());
        double fs_time = 48000.0;
        // Eğer zaman vektörü doluysa son süreden hesapla, yoksa 48000 varsay
        if (!timeVec.isEmpty() && timeVec.last() > 0)
            fs_time = rawSignal.size() / timeVec.last();

        for(int i=0; i < rawSignal.size(); ++i) timeVec[i] = i / fs_time;
    }

    // 2. GİRİŞ GRAFİKLERİ (ÜST)
    m_origTimePlot->updatePlot(timeVec, rawSignal);
    m_origTimePlot->getPlot()->xAxis->setRange(0, timeVec.last());

    // Frekansı hesapla (Veri / Süre)
    double fs = (timeVec.last() > 0) ? (rawSignal.size() / timeVec.last()) : 48000.0;
    plotFFT(rawSignal, fs, m_origFreqPlot->getPlot());

    // 3. ÇIKIŞ GRAFİKLERİ (ALT)
    // Eğer noisySignal (işlenmiş sinyal) BOŞ ise grafikleri TEMİZLE.
    if (noisySignal.isEmpty()) {
        // Filtreli sinyal yoksa grafikleri temizle
        m_filteredTimePlot->getPlot()->graph(0)->data()->clear();
        m_filteredTimePlot->getPlot()->replot();

        m_filteredFreqPlot->getPlot()->graph(0)->data()->clear();
        m_filteredFreqPlot->getPlot()->replot();

        // İstatistikleri sıfırla (Label isimlerini bilmediğimiz için boş vektör gönderiyoruz)
        QVector<double> empty;
        updateStats(empty);
    }
    else {
        // Filtreli sinyal varsa çiz
        m_filteredTimePlot->updatePlot(timeVec, noisySignal);
        m_filteredTimePlot->getPlot()->xAxis->setRange(0, timeVec.last());

        plotFFT(noisySignal, fs, m_filteredFreqPlot->getPlot());

        // İstatistikleri güncelle
        updateStats(noisySignal);
    }
}

/*
// Bu fonksiyon: Verilen sinyali alır, senin FFTProcessor sınıfını kullanarak işler ve grafiğe basar.
void MainWindow::plotFFT(const QVector<double> &signal, double fs, QCustomPlot *plot)
{
    if (signal.isEmpty()) return;

    // 1. Çıktı vektörlerini hazırla
    QVector<double> freqVec;
    QVector<double> magVec;

    // 2. UI'dan Ayarları Al (Eğer UI elemanların farklı isimdeyse buraları düzelt)
    // Varsayılan olarak RECTANGULAR (Penceresiz) ve Lineer kabul ediyoruz.
    // Eğer UI'da bu ayarlar varsa şöyle alabilirsin:

    // WindowType window = static_cast<WindowType>(ui->cmbFFTWindow->currentIndex());
    WindowType window = WindowType::RECTANGULAR; // Şimdilik varsayılan

    // bool useDB = (ui->cmbFFTScale->currentText() == "dB" veya "Logarithmic");
    bool useDB = false; // Şimdilik varsayılan (Lineer)


    // 3. SENİN SINIFINI ÇAĞIR (computeFFT)
    // Sınıfın statik olduğu için nesne üretmeden direkt çağırıyoruz.
    FFTProcessor::computeFFT(signal, fs, freqVec, magVec, window);

    // 4. dB Dönüşümü İsteniyorsa Uygula
    if (useDB) {
        FFTProcessor::applyMagnitudeScaling(magVec, true);
    }

    // 5. Grafiğe Çizdir
    plot->graph(0)->setData(freqVec, magVec);

    // Eksenleri Otomatik Ayarla
    plot->xAxis->setRange(0, fs / 2); // Nyquist Frekansına kadar göster

    // Y Eksenini ayarla (dB ise genelde alt sınır -100 vs olur, lineer ise 0)
    if (useDB)
        plot->yAxis->setRange(-120, *std::max_element(magVec.begin(), magVec.end()) + 10);
    else
        plot->rescaleAxes(); // Lineer ise otomatiğe bırak

    plot->replot();
} */




void MainWindow::plotFFT(const QVector<double> &signal, double fs, QCustomPlot *plot)
{
    // Güvenlik: Sinyal boşsa veya grafik yoksa çık
    if (signal.isEmpty() || !plot) return;

    QVector<double> freqVec, magVec;

    // 1. UI Ayarlarını Al
    // (Eğer UI eleman isimlerin farklıysa lütfen buraları düzelt)
    WindowType window = WindowType::RECTANGULAR;
    if (ui->cmbFFTScale)
        window = static_cast<WindowType>(ui->cmbFFTScale->currentIndex());

    // "dB" kelimesi geçiyor mu kontrol et
    bool useDB = false;
    if (ui->cmbFFTScale)
        useDB = ui->cmbFFTScale->currentText().contains("dB", Qt::CaseInsensitive);

    // 2. Hesapla (Senin Sınıfın)
    FFTProcessor::computeFFT(signal, fs, freqVec, magVec, window);

    // 3. dB Dönüşümü (Güvenli Mod)
    if (useDB) {
        for(int i=0; i<magVec.size(); ++i) {
            double val = magVec[i];
            if (val < 1e-12) val = 1e-12; // log(0) hatasını önle
            magVec[i] = 20.0 * std::log10(val);
        }
    }

    // 4. Grafiğe Veriyi Koy
    plot->graph(0)->setData(freqVec, magVec);

    // 5. EKSENLERİ AYARLA (SORUNUN ÇÖZÜMÜ BURASI)
    plot->xAxis->setRange(0, fs / 2); // Nyquist sınırı

    // Y eksenini veriye göre otomatik ayarla
    plot->graph(0)->rescaleValueAxis(false, true);

    // Biraz pay bırak (Görüntü sıkışmasın)
    if (useDB) {
        // dB modunda tavanı biraz artır
        double maxVal = -100;
        if (!magVec.isEmpty()) maxVal = *std::max_element(magVec.begin(), magVec.end());
        plot->yAxis->setRange(maxVal - 140, maxVal + 10);
    } else {
        // Lineer Mod: Otomatik ölçekle ama alt sınırı 0 yap
        plot->rescaleAxes();
        plot->yAxis->setRangeLower(0);
    }

    plot->replot();
}



void MainWindow::on_chkRealTime_checkStateChanged(const Qt::CheckState &arg1)
{

}


void MainWindow::on_btnReverb_clicked()
{
    // 1. Sinyal Kontrolü
    if (rawSignal.isEmpty()) {
        ui->statusbar->showMessage("İşlenecek sinyal yok!");
        return;
    }

    // 2. Ses Motorunu Durdur (Güvenlik)
    stopAudio();

    // 3. UNDO (GERİ AL) KAYDI
    if (undoStack.size() > 10) undoStack.removeFirst();
    UndoState state;
    state.signalData = !noisySignal.isEmpty() ? noisySignal : rawSignal;
    state.type = UndoFilter; // Veya yeni bir UndoType::Effect ekleyebilirsin
    undoStack.push(state);


    // 4. İŞLENECEK SİNYALİ SEÇ
    // Zincirleme etki: Eğer zaten gürültülü veya filtreli bir ses varsa onun üzerine yankı ekle.
    QVector<double> inputSignal = !noisySignal.isEmpty() ? noisySignal : rawSignal;

    // 5. PARAMETRELER (İstersen Slider'dan alabilirsin)
    double delayTime = 0.4; // 0.4 Saniye (Büyük oda etkisi)
    double decay = 0.5;     // Her yankıda ses %50 azalsın

    // Gerçek Frekansı Bul
    double fs = 48000.0;
    if (!timeVec.isEmpty() && timeVec.last() > 0)
        fs = rawSignal.size() / timeVec.last();


    // 6. EFEKTİ UYGULA
    // Sonucu noisySignal'e yazıyoruz ki Play Output butonu bunu çalsın.
    FilterProcessor::applyDelay(inputSignal, noisySignal, fs, delayTime, decay);


    // 7. GRAFİKLERİ GÜNCELLE updateAllGraphs();
    updateAllGraphs();

    ui->btnReverb->setText("Reverb (Aktif)");

    ui->statusbar->showMessage("Reverberasyon (Yankı) efekti uygulandı. 🎸");
}





