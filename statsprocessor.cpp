#include "statsprocessor.h"
#include <cmath>   // sqrt, pow için
#include <limits>  // numeric_limits için
#include "statsprocessor.h"

StatsProcessor::StatsProcessor() {}

StatsProcessor::SignalStats StatsProcessor::computeStats(const QVector<double> &signal)
{
    // Başlangıçta boş değerler
    SignalStats stats = {0.0, 0.0, 0.0, 0.0};

    if (signal.isEmpty()) return stats;

    double sum = 0.0;
    double sqSum = 0.0; // Kareler toplamı (RMS için)

    // Max için en küçük, Min için en büyük sayıyı verelim ki ilk veri bunları ezsin.
    stats.maxVal = -std::numeric_limits<double>::max();
    stats.minVal = std::numeric_limits<double>::max();

    for (double val : signal) {
        // 1. Max Bul
        if (val > stats.maxVal) stats.maxVal = val;

        // 2. Min Bul
        if (val < stats.minVal) stats.minVal = val;

        // 3. Toplamları Al
        sum += val;
        sqSum += (val * val);
    }

    int N = signal.size();
    if (N > 0) {
        stats.meanVal = sum / N; // Aritmetik Ortalama
        stats.rmsVal = std::sqrt(sqSum / N); // Karekök(Kareler Ortalaması)
    }

    return stats;
}
