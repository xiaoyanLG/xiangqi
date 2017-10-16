#-------------------------------------------------
#
# Project created by QtCreator 2017-10-10T11:03:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xiangqiWidget
TEMPLATE = app

INCLUDEPATH += socket

SOURCES += main.cpp\
        mainwindow.cpp \
    xyqipanwidget.cpp \
    xybattleinfowidget.cpp \
    xyqiziwidget.cpp \
    xyqibu.cpp \
    socket/client.cpp \
    socket/connection.cpp \
    socket/peermanager.cpp \
    socket/server.cpp \
    socket/xyudpbroadcast.cpp \
    xyqishou.cpp \
    socket/xytcpserver.cpp

HEADERS  += mainwindow.h \
    xyqipanwidget.h \
    xybattleinfowidget.h \
    xyqiziwidget.h \
    xyqibu.h \
    socket/client.h \
    socket/connection.h \
    socket/peermanager.h \
    socket/server.h \
    socket/xyudpbroadcast.h \
    xyqishou.h \
    socket/xytcpserver.h

FORMS    += mainwindow.ui

RESOURCES += \
    images/images.qrc

RC_FILE = ico.rc
