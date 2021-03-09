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
    MyUdpRtp.cpp \
    RtmpRecvFlv.cpp \
    RtmpSendFlv.cpp \
    MyFFmpeg.cpp

HEADERS  += mainwindow.h \
    MyPixel.h \
    MyPcm.h \
    MyH264.h \
    MyAac.h \
    MyFlv.h \
    MyUdpRtp.h \
    RtmpRecvFlv.h \
    RtmpSendFlv.h \
    MyFFmpeg.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/
LIBS += -lrtmp -lavformat -lavcodec -lavutil -lswresample -lx264 -lx265 -lmp3lame -pthread -lz -lbz2

RESOURCES += \
    easyplayer.qrc
