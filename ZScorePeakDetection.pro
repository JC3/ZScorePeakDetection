#==============================================================================
# Author:  Jason Cipriani
# Website: https://github.com/JC3/ZScorePeakDetection
# License: https://github.com/JC3/ZScorePeakDetection/blob/master/LICENSE
#==============================================================================

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

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    sample.txt \
    sample_powerspectrum.txt
