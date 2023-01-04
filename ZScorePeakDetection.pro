#==============================================================================
# Author:  Jason Cipriani
# Website: https://github.com/JC3/ZScorePeakDetection
# License: https://github.com/JC3/ZScorePeakDetection/blob/master/LICENSE
#==============================================================================

QMAKE_TARGET_DESCRIPTION = "Z-Score Peak Detection"
VERSION = 1.0.0.0

QT       += core gui widgets charts

CONFIG += c++17

SOURCES += \
    areazoomwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    thresholdingdemo.cpp

HEADERS += \
    areazoomwidget.h \
    mainwindow.h \
    thresholdingdemo.h

FORMS += \
    mainwindow.ui


unix:application.path = /opt/bin
unix:application.files = ZScorePeakDetection
unix:INSTALLS += application
win32:RC_ICONS = chart.ico

DISTFILES += \
    sample.txt \
    sample_powerspectrum.txt
