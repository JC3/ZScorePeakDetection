//=============================================================================
/**
 * Author:  Jason Cipriani
 * Website: https://github.com/JC3/ZScorePeakDetection
 * License: https://github.com/JC3/ZScorePeakDetection/blob/master/LICENSE
 */
//=============================================================================

#ifndef AREAZOOMWIDGET_H
#define AREAZOOMWIDGET_H

#include <QWidget>

class AreaZoomWidget : public QWidget {
    Q_OBJECT
public:
    explicit AreaZoomWidget(QWidget *parent = nullptr);
    float zoomFrom () const { return from_; }
    float zoomTo () const { return to_; }
protected:
    void mousePressEvent (QMouseEvent *event);
    void mouseMoveEvent (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);
    void paintEvent (QPaintEvent *event);
public slots:
    void setZoomWindow (float from, float to) { from_ = from; to_ = to; validate(); update(); }
    void setZoomWindowFrom (float from) { from_ = from; validate(); update(); }
    void setZoomWindowTo (float to) { to_ = to; validate(); update(); }
signals:
    void zoomed (float from, float to);
private:
    float from_, to_;
    bool dragging_;
    QPoint dragstart_;
    float dragfrom_, dragto_;
    void dragUpdate (QPoint pos);
    QRect zoomWindowRect () const;
    void validate ();
};

#endif // AREAZOOMWIDGET_H
