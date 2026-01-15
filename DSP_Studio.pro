QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fftprocessor.cpp \
    filterprocessor.cpp \
    main.cpp \
    mainwindow.cpp \
    noiseprocessor.cpp \
    plotmanager.cpp \
    qcustomplot.cpp \
    signalgenerator.cpp \
    statsprocessor.cpp \
    utils.cpp

HEADERS += \
    fftprocessor.h \
    filterprocessor.h \
    mainwindow.h \
    noiseprocessor.h \
    plotmanager.h \
    qcustomplot.h \
    signalgenerator.h \
    statsprocessor.h \
    utils.h

FORMS += \
    mainwindow.ui

# ---- OpenCV Paths ----
INCLUDEPATH += D:/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/include
LIBS += -LD:/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/bin \
    -lopencv_core455 \
    -lopencv_imgproc455 \
    -lopencv_highgui455 \
    -lopencv_imgcodecs455 \
    -lopencv_videoio455

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
