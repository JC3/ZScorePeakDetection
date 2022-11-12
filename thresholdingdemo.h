//=============================================================================
/**
 * Author:  Jason Cipriani
 * Website: https://github.com/JC3/ZScorePeakDetection
 * License: https://github.com/JC3/ZScorePeakDetection/blob/master/LICENSE
 *
 * This is an implementation of the Robust Peak Detection Algorithm Using
 * Z-Scores (Brackel, J.P.G. van) from https://stackoverflow.com/a/22640362.
 */
//=============================================================================

#ifndef THRESHOLDINGDEMO_H
#define THRESHOLDINGDEMO_H

#include <QObject>
#include <QVector>

class ThresholdingDemo : public QObject {
    Q_OBJECT
public:    
    struct Params {
        int lag;
        float threshold;
        float influence;
        Params () : lag(30), threshold(5), influence(0) { }
    };
    struct Output {
        QVector<float> input;
        Params params;
        QVector<float> mean;
        QVector<float> stddev;
        QVector<int> signal;
        void clear () { input.clear(); mean.clear(); stddev.clear(); signal.clear(); }
    };
    explicit ThresholdingDemo (QObject *parent = nullptr);
    Params params () const { return params_; }
public slots:
    void setInput (QVector<float> input) { input_ = input; update(); }
    void setLag (int lag) { params_.lag = lag; update(); }
    void setThreshold (double threshold) { params_.threshold = threshold; update(); }
    void setInfluence (double influence) { params_.influence = influence; update(); }
signals:
    void outputChanged (ThresholdingDemo::Output output);
private:
    QVector<float> input_;
    Params params_;
    void update ();
    static void threshold (const Params &params, Output &output, const QVector<float> &input);
};

#endif // THRESHOLDINGDEMO_H
