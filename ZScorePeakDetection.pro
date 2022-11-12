QT       += core gui widgets charts

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    thresholdingdemo.cpp

HEADERS += \
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
