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

void ThresholdingDemo::threshold (const Params &params,
                                  Output &output,
                                  const QVector<float> &input)
{

    // adapted from https://stackoverflow.com/a/46956908/616460

    static const auto mean = [](const QVector<float> &vec, int from, int to) {
        float sum = 0;
        for (int k = from; k <= to; ++ k) sum += vec[k];
        return sum / (to - from + 1);
    };

    static const auto stdDev = [](const QVector<float> &vec, float mean, int from, int to) {
        float sum = 0;
        for (int k = from; k <= to; ++ k) sum += std::pow(vec[k] - mean, 2);
        return std::sqrt(sum / (to - from + 1));
    };

    int lag = params.lag;
    float threshold = params.threshold;
    float influence = params.influence;

    output.clear();

    if (input.size() <= lag + 2)
        return;

    //Initialise variables
    QVector<int> signls(input.size(), 0.0);
    QVector<float> avgFilter(input.size(), 0.0);
    QVector<float> stdFilter(input.size(), 0.0);
    QVector<float> filteredY = input;
    avgFilter[lag] = mean(filteredY, 0, lag);
    stdFilter[lag] = stdDev(filteredY, avgFilter[lag], 0, lag);

    for (auto i = lag + 1; i < input.size(); i++)
    {
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

    output.input = input;
    output.params = params;
    output.mean = avgFilter;
    output.stddev = stdFilter;
    output.signal = signls;

}
