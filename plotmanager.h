#ifndef PLOTMANAGER_H
#define PLOTMANAGER_H

#include "qcustomplot.h"
#include <QVector>
#include <QString>

class PlotManager
{
public:
    // Kurucu fonksiyon: Hangi QCustomPlot widget'ını yöneteceğini parametre olarak alır
    explicit PlotManager(QCustomPlot *plotWidget);

    // Grafiğin başlığını, eksen isimlerini ayarlar
    void setupPlot(const QString &title, const QString &xLabel, const QString &yLabel);

    // Veriyi grafiğe basar
    void updatePlot(const QVector<double> &x, const QVector<double> &y);

    // Grafiği temizler
    void clearPlot();

    QCustomPlot* getPlot() const { return plot; }


private:
    QCustomPlot *plot; // Yönettiğimiz grafik aracı

};

#endif // PLOTMANAGER_H
