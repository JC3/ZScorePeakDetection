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
 *    - Absolute minimum level of a peak.
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
        // extra experimental options
        bool reverse;
        float minlevel;
        Params () : lag(30), threshold(5), influence(0), reverse(false), minlevel(0) { }
    };
    struct Output {
        QVector<float> input;
        Params params;
        QVector<float> mean;
        QVector<float> stddev;
        QVector<int> signal;
        int outfrom, outto; // mean,stddev,signal valid indices, INCLUSIVE
        void clear () { input.clear(); mean.clear(); stddev.clear(); signal.clear(); outfrom = 0; outto = -1; }
    };
    explicit ThresholdingDemo (QObject *parent = nullptr);
    Params params () const { return params_; }
    int size () const { return input_.size(); }
public slots:
    void setInput (QVector<float> input) { input_ = input; update(); }
    void setLag (int lag) { params_.lag = lag; update(); }
    void setThreshold (double threshold) { params_.threshold = threshold; update(); }
    void setInfluence (double influence) { params_.influence = influence; update(); }
    void setReverse (bool reverse) { params_.reverse = reverse; update(); }
    void setMinLevel (double minlevel) { params_.minlevel = minlevel; update(); }
signals:
    void outputChanged (ThresholdingDemo::Output output);
private:
    QVector<float> input_;
    Params params_;
    void update ();
    static void threshold (const Params &params, Output &output, QVector<float> input);
};

#endif // THRESHOLDINGDEMO_H
