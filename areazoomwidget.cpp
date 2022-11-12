//=============================================================================
/**
 * Author:  Jason Cipriani
 * Website: https://github.com/JC3/ZScorePeakDetection
 * License: https://github.com/JC3/ZScorePeakDetection/blob/master/LICENSE
 */
//=============================================================================

#include "areazoomwidget.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <cmath>


AreaZoomWidget::AreaZoomWidget (QWidget *parent)
    : QWidget(parent),
      from_(0),
      to_(1),
      dragging_(false)
{
    setCursor(Qt::SizeAllCursor);
}


void AreaZoomWidget::mousePressEvent (QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging_ = true;
        dragstart_ = event->pos();
        dragfrom_ = from_;
        dragto_ = to_;
        qDebug() << dragfrom_ << dragto_;
        dragUpdate(dragstart_);
        event->accept();
    } else {
        event->ignore();
    }
}


void AreaZoomWidget::mouseMoveEvent (QMouseEvent *event) {
    if (dragging_) {
        dragUpdate(event->pos());
        event->accept();
    } else {
        event->ignore();
    }
}


void AreaZoomWidget::mouseReleaseEvent (QMouseEvent *event) {
    if (dragging_ && event->button() == Qt::LeftButton) {
        dragUpdate(event->pos());
        dragging_ = false;
        event->accept();
    } else {
        event->ignore();
    }
}


void AreaZoomWidget::dragUpdate (QPoint pos) {

    float xoff = pos.x() - dragstart_.x();
    float yoff = pos.y() - dragstart_.y();
    float zcenter = (dragfrom_ + dragto_) / 2.0;
    float zwidth = dragto_ - dragfrom_;

    zcenter += xoff / width();
    zwidth *= pow(2, yoff / (width() / 8));
    if (zwidth > 1.0f) zwidth = 1.0f;
    float newfrom = zcenter - zwidth / 2;
    float newto = zcenter + zwidth / 2;

    setZoomWindow(newfrom, newto);
    emit zoomed(from_, to_);

}


void AreaZoomWidget::paintEvent (QPaintEvent *event) {

    QColor bg = palette().color(QPalette::Window);
    QColor bgbrd = palette().color(QPalette::WindowText);
    QColor fg = palette().color(QPalette::Button);
    QColor fgbrd = palette().color(QPalette::ButtonText);
    QRect zoomrc = zoomWindowRect();

    {
        QPainter p(this);
        p.fillRect(rect(), bg);
        p.fillRect(zoomrc, fg);
        p.setPen(fgbrd);
        p.drawRect(zoomrc.adjusted(0, 0, -1, -1));
        p.setPen(bgbrd);
        p.drawRect(rect().adjusted(0, 0, -1, -1));
    }

    event->accept();

}


QRect AreaZoomWidget::zoomWindowRect () const {

    int x1 = qRound(from_ * width());
    int x2 = qRound(to_ * width());
    if (x1 < 0) x1 = 0;
    if (x2 >= width()) x2 = width() - 1;

    return QRect(QPoint(x1, 0), QPoint(x2, height() - 1));

}


void AreaZoomWidget::validate () {

    if (to_ < from_) std::swap(from_, to_);
    float width = to_ - from_;
    if (width > 1.0) width = 1.0;
    if (from_ < 0.0) { from_ = 0; to_ = width; }
    if (to_ > 1.0) { to_ = 1.0; from_ = to_ - width; }

}
