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
    win_qextserialport.cpp \
    aboutdialog.cpp

HEADERS  += wincommainwindow.h \
    qextserialbase.h \
    qextserialport.h \
    win_qextserialport.h \
    aboutdialog.h

FORMS    += wincommainwindow.ui \
    aboutdialog.ui
RC_FILE += wincom.rc

RESOURCES += \
    resource/resource.qrc
