#-------------------------------------------------
#
# Project created by QtCreator 2016-09-13T20:38:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wincom
TEMPLATE = app


SOURCES += main.cpp\
        wincommainwindow.cpp \
    qextserialbase.cpp \
    qextserialport.cpp \
    win_qextserialport.cpp

HEADERS  += wincommainwindow.h \
    qextserialbase.h \
    qextserialport.h \
    win_qextserialport.h

FORMS    += wincommainwindow.ui
RC_FILE += wincom.rc
