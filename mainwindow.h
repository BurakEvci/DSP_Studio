#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotmanager.h"
#include "signalgenerator.h"
#include "noiseprocessor.h"
#include "fftprocessor.h"
#include "filterprocessor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

};
#endif // MAINWINDOW_H
