#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "thresholdingdemo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow (QWidget *parent = nullptr);
    ~MainWindow ();
private slots:
    void displayOutput (ThresholdingDemo::Output output);
    void on_actDataReset_triggered();
    void on_actDataImport_triggered();
signals:
    void dataChanged (QVector<float> data);
private:
    Ui::MainWindow *ui_;
    ThresholdingDemo *demo_;
};
#endif // MAINWINDOW_H
