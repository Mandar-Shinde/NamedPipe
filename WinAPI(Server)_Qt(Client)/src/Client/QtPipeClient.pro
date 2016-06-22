#-------------------------------------------------
#
# Project created by Mandar 2016-06-22T12:49:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtPipeClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    localsocketipcclient.cpp

HEADERS  += mainwindow.h \
    localsocketipcclient.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
