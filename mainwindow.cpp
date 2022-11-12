//=============================================================================
/**
 * Author:  Jason Cipriani
 * Website: https://github.com/JC3/ZScorePeakDetection
 * License: https://github.com/JC3/ZScorePeakDetection/blob/master/LICENSE
 */
//=============================================================================

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChartView>
#include <QDebug>
#include <QAreaSeries>
#include <QLineSeries>
#include <QValueAxis>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>

using namespace QtCharts;

static const QVector<float> InitialData = {
    1,1,1.1f,1,0.9f,1,1,1.1f,1,0.9f,1,1.1f,1,1,0.9f,1,1,1.1f,1,1,
    1,1,1.1f,0.9f,1,1.1f,1,1,0.9f,1,1.1f,1,1,1.1f,1,0.8f,0.9f,1,1.2f,0.9f,1,
    1,1.1f,1.2f,1,1.5f,1,3,2,5,3,2,1,1,1,0.9f,1,
    1,3,2.6f,4,3,3.2f,2,1,1,0.8f,4,4,2,2.5f,1,1,1
};

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow(parent),
    ui_(new Ui::MainWindow)
{

    ui_->setupUi(this);
    ui_->chvAlgo->setRenderHint(QPainter::Antialiasing);
    ui_->chvOut->setRenderHint(QPainter::Antialiasing);
    demo_ = new ThresholdingDemo(this);

    ThresholdingDemo::Params cur = demo_->params();
    ui_->pLag->setValue(cur.lag);
    ui_->pThreshold->setValue(cur.threshold);
    ui_->pInfluence->setValue(cur.influence);

    connect(this, &MainWindow::dataChanged, demo_, &ThresholdingDemo::setInput);
    connect(ui_->pLag, SIGNAL(valueChanged(int)), demo_, SLOT(setLag(int)));
    connect(ui_->pThreshold, SIGNAL(valueChanged(double)), demo_, SLOT(setThreshold(double)));
    connect(ui_->pInfluence, SIGNAL(valueChanged(double)), demo_, SLOT(setInfluence(double)));
    connect(demo_, &ThresholdingDemo::outputChanged, this, &MainWindow::displayOutput);
    connect(this, &MainWindow::dataChanged, [this]() { ui_->zoom->setZoomWindow(0, 1); });

    ui_->actDataReset->trigger();

}

MainWindow::~MainWindow () {

    delete ui_;

}

template <typename T>
static QLineSeries * createLineSeries (const QVector<T> &values,
                                       int from = 0,
                                       const QVector<T> &offsets = QVector<T>(),
                                       T offsetmul = (T)1)
{
    QLineSeries *series = new QLineSeries();
    for (auto k = from; k < values.size(); ++ k)
        *series << QPointF(k, values[k] + (offsets.empty() ? 0 : offsetmul * offsets[k]));
    return series;
}

void MainWindow::displayOutput (ThresholdingDemo::Output output) {

    static const auto cleared = [](QChart *chart) {
        chart->removeAllSeries();
        auto axes = chart->axes();
        for (auto axis : axes) chart->removeAxis(axis);
        return chart;
    };

    // this one saves typing
    static const auto va = [](QChart *chart, Qt::Orientation dir) {
        return qobject_cast<QValueAxis*>(chart->axes(dir).first());
    };

    QChart *achart = cleared(ui_->chvAlgo->chart());
    QChart *ochart = cleared(ui_->chvOut->chart());

    QLineSeries *input = createLineSeries(output.input);
    QLineSeries *mean = createLineSeries(output.mean, output.params.lag);
    QLineSeries *hi = createLineSeries(output.mean, output.params.lag, output.stddev, output.params.threshold);
    QLineSeries *lo = createLineSeries(output.mean, output.params.lag, output.stddev, -output.params.threshold);
    QAreaSeries *hilo = new QAreaSeries(hi, lo);
    QLineSeries *signal = createLineSeries(output.signal);

    input->setColor(Qt::blue);
    mean->setColor(Qt::cyan);
    hilo->setBorderColor(Qt::green);
    hilo->setColor(QColor::fromRgbF(0.9, 0.9, 0.9));
    signal->setColor(Qt::red);

    achart->addSeries(hilo);
    achart->addSeries(mean);
    achart->addSeries(input);
    ochart->addSeries(signal);

    achart->createDefaultAxes();
    ochart->createDefaultAxes();
    QValueAxis *ax = va(achart, Qt::Horizontal), *ay = va(achart, Qt::Vertical);
    QValueAxis *ox = va(ochart, Qt::Horizontal), *oy = va(ochart, Qt::Vertical);

    int interval = std::max(1, qRound(output.input.size() / 40.0) * 10);
    ax->setMin(0);
    ax->setTickType(QValueAxis::TicksDynamic);
    ax->setTickInterval(interval);
    ax->setLabelFormat("%d");
    ox->setMin(0);
    ox->setTickType(QValueAxis::TicksDynamic);
    ox->setTickInterval(interval);
    ox->setLabelFormat("%d");
    ay->setMin(0);
    oy->setRange(-1.5, 1.5);
    achart->legend()->setVisible(false);
    ochart->legend()->setVisible(false);

}


void MainWindow::on_zoom_zoomed (float from, float to) {

    static const auto va = [](QChart *chart, Qt::Orientation dir) {
        return qobject_cast<QValueAxis*>(chart->axes(dir).first());
    };

    int count = demo_->size();
    int minx = qRound(from * count);
    int maxx = qRound(to * count);

    QValueAxis *ax = va(ui_->chvAlgo->chart(), Qt::Horizontal);
    QValueAxis *ox = va(ui_->chvOut->chart(), Qt::Horizontal);
    ax->setRange(minx, maxx);
    ox->setRange(minx, maxx);

}


void MainWindow::on_actDataReset_triggered () {

    emit dataChanged(InitialData);

}


void MainWindow::on_actDataImport_triggered () {

    QString filename = QFileDialog::getOpenFileName(this, "Import Data");
    if (filename == "")
        return;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, "Error Opening File", file.errorString());
        return;
    }

    QTextStream str(&file);
    QVector<float> data;
    float value;
    for (str >> value; !str.atEnd(); str >> value)
        data.append(value);

    if (data.empty()) {
        QMessageBox::critical(this, "Error Loading File", "No data found.");
        return;
    }

    emit dataChanged(data);

    if (data.size() <= demo_->params().lag)
        QMessageBox::information(this, "Note", "Lag is currently greater than the size of your data set.\nYou won't see anything until you decrease it.");

}


void MainWindow::on_actLinkAlgorithm_triggered()
{
    QDesktopServices::openUrl(QUrl("https://stackoverflow.com/a/22640362"));
}


void MainWindow::on_actLinkGitlab_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/JC3/ZScorePeakDetection"));
}

