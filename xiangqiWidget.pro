#-------------------------------------------------
#
# Project created by QtCreator 2017-10-10T11:03:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xiangqiWidget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xyqipanwidget.cpp \
    xybattleinfowidget.cpp \
    xyqiziwidget.cpp \
    xyqibu.cpp

HEADERS  += mainwindow.h \
    xyqipanwidget.h \
    xybattleinfowidget.h \
    xyqiziwidget.h \
    xyqibu.h

FORMS    += mainwindow.ui

RESOURCES += \
    images/images.qrc

RC_FILE = ico.rc
