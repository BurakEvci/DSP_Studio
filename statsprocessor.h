#ifndef STATSPROCESSOR_H
#define STATSPROCESSOR_H

#include <QVector>

class StatsProcessor
{
public:
    StatsProcessor();

    // Verileri paketleyip döndürmek için bir yapı (Struct)
    struct SignalStats {
        double maxVal;
        double minVal;
        double meanVal; // DC Offset
        double rmsVal;  // Güç
    };

    static SignalStats computeStats(const QVector<double> &signal);
};

#endif // STATSPROCESSOR_H
