//=============================================================================
/**
 * Author:  Jason Cipriani
 * Website: https://github.com/JC3/ZScorePeakDetection
 * License: https://github.com/JC3/ZScorePeakDetection/blob/master/LICENSE
 */
//=============================================================================

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
