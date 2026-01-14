#include "plotmanager.h"

PlotManager::PlotManager(QCustomPlot *plotWidget) : plot(plotWidget)
{
    // Temel ayarlar (Zoom ve Sürükleme aktif olsun)
    if(plot) {
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        plot->addGraph(); // Varsayılan olarak 0. indexli grafiği ekle
    }
}

void PlotManager::setupPlot(const QString &title, const QString &xLabel, const QString &yLabel)
{
    if(!plot) return;

    // Eksen isimleri
    plot->xAxis->setLabel(xLabel);
    plot->yAxis->setLabel(yLabel);

    // Eğer başlık eklemek istersen (QCustomPlot'ta title biraz farklı eklenir ama şimdilik eksen yeterli)
    // plot->plotLayout()->insertRow(0);
    // plot->plotLayout()->addElement(0, 0, new QCPTextElement(plot, title, QFont("sans", 12, QFont::Bold)));
}

void PlotManager::updatePlot(const QVector<double> &x, const QVector<double> &y)
{
    if(!plot) return;

    // 0. grafiğe veriyi ata
    plot->graph(0)->setData(x, y);

    // Eksenleri veriye göre otomatik ayarla
    plot->rescaleAxes();

    // Çizimi yenile
    plot->replot();
}

void PlotManager::clearPlot()
{
    if(!plot) return;

    // Veriyi temizle
    plot->graph(0)->data()->clear();
    plot->replot();
}

//QCustomPlot nesnelerini günceller, zaman ve frekans grafiklerini çizer
