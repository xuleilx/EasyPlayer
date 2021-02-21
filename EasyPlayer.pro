#-------------------------------------------------
#
# Project created by QtCreator 2021-02-04T19:24:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasyPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyPixel.cpp \
    MyPcm.cpp \
    MyH264.cpp \
    MyAac.cpp \
    MyFlv.cpp \
    MyUdpRtp.cpp

HEADERS  += mainwindow.h \
    MyPixel.h \
    MyPcm.h \
    MyH264.h \
    MyAac.h \
    MyFlv.h \
    MyUdpRtp.h

FORMS    += mainwindow.ui

RESOURCES += \
    easyplayer.qrc
