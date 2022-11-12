//=============================================================================
/**
 * Author:  Jason Cipriani
 * Website: https://github.com/JC3/ZScorePeakDetection
 * License: https://github.com/JC3/ZScorePeakDetection/blob/master/LICENSE
 *
 * This is an implementation of the Robust Peak Detection Algorithm Using
 * Z-Scores (Brackel, J.P.G. van) from https://stackoverflow.com/a/22640362
 * with some experimental modifications by me:
 *
 *    - Option to process data set in reverse.
 *    - Option to find peaks in first [lag] data points.
 */
//=============================================================================

#include "thresholdingdemo.h"
#include <QMetaObject>
#include <cmath>

ThresholdingDemo::ThresholdingDemo (QObject *parent)
    : QObject(parent)
{

    qRegisterMetaType<ThresholdingDemo::Output>("ThresholdingDemo::Output");

}

void ThresholdingDemo::update () {

    Output output;
    threshold(params_, output, input_);
    emit outputChanged(output);

}

template <typename T>
static QVector<T> & reverseif (QVector<T> &vec, bool cond) {
    if (cond)
        std::reverse(vec.begin(), vec.end());
    return vec;
}

void ThresholdingDemo::threshold (const Params &params,
                                  Output &output,
                                  QVector<float> input)
{

    // adapted from https://stackoverflow.com/a/46956908/616460.
    // there were some bugs in that implementation which have been corrected here,
    // as well as porting std::vector to QVector and some API changes.

    static const auto mean = [](const QVector<float> &vec, int from, int to) {
        from = std::max(0, from);
        float sum = 0;
        for (int k = from; k <= to; ++ k) sum += vec[k];
        return sum / (to - from + 1);
    };

    static const auto stdDev = [](const QVector<float> &vec, float mean, int from, int to) {
        from = std::max(0, from);
        float sum = 0;
        for (int k = from; k <= to; ++ k) sum += std::pow(vec[k] - mean, 2);
        return std::sqrt(sum / (to - from + 1));
    };

    reverseif(input, params.reverse);

    int lag = params.lag;
    float threshold = params.threshold;
    float influence = params.influence;

    output.clear();

    if (input.size() <= lag + 2)
        return;

    QVector<int> signls(input.size(), 0.0);
    QVector<float> avgFilter(input.size(), 0.0);
    QVector<float> stdFilter(input.size(), 0.0);
    QVector<float> filteredY = input;
    avgFilter[lag] = mean(filteredY, 0, lag);
    stdFilter[lag] = stdDev(filteredY, avgFilter[lag], 0, lag);

    for (auto i = lag + 1; i < input.size(); i++) {
        if (std::abs(input[i] - avgFilter[i-1]) > threshold * stdFilter[i-1]) {
            signls[i] = (input[i] > avgFilter[i-1]) ? 1 : -1;
            filteredY[i] = influence* input[i] + (1 - influence) * filteredY[i-1];
        } else {
            signls[i] = 0; //# No signal
            filteredY[i] = input[i];
        }
        avgFilter[i] = mean(filteredY, i-lag, i);
        stdFilter[i] = stdDev(filteredY, avgFilter[i], i-lag, i);
    }

    output.input = reverseif(input, params.reverse);
    output.params = params;
    output.mean = reverseif(avgFilter, params.reverse);
    output.stddev = reverseif(stdFilter, params.reverse);
    output.signal = reverseif(signls, params.reverse);
    output.outfrom = lag;
    output.outto = input.size() - 1;

    if (params.reverse) {
        output.outfrom = input.size() - output.outfrom - 1;
        output.outto = input.size() - output.outto - 1;
        std::swap(output.outfrom, output.outto);
    }

}
