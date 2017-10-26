#-------------------------------------------------
#
# Project created by QtCreator 2017-10-10T11:03:33
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xiangqiWidget
TEMPLATE = app

INCLUDEPATH += socket chineseInput

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
    socket/xytcpserver.cpp \
    chineseInput/xybordershadowwidget.cpp \
    chineseInput/xydatabaseoperation.cpp \
    chineseInput/xyinput.cpp \
    chineseInput/xymenu.cpp \
    chineseInput/xymenustyle.cpp \
    chineseInput/xytooltips.cpp \
    chineseInput/xytranslateitem.cpp \
    chineseInput/xytranslatemodel.cpp \
    chineseInput/xytranslateview.cpp \
    xyaiqishou.cpp \
    xyqipanstatus.cpp \
    wushi.cpp \
    xythinkingbox.cpp

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
    socket/xytcpserver.h \
    chineseInput/xybordershadowwidget.h \
    chineseInput/xydatabaseoperation.h \
    chineseInput/xyinput.h \
    chineseInput/xymenu.h \
    chineseInput/xymenustyle.h \
    chineseInput/xytooltips.h \
    chineseInput/xytranslateitem.h \
    chineseInput/xytranslatemodel.h \
    chineseInput/xytranslateview.h \
    xyaiqishou.h \
    xyqipanstatus.h \
    xyaivalue.h \
    wushi.h \
    xythinkingbox.h

FORMS    += mainwindow.ui

RESOURCES += \
    images/images.qrc \
    db/db.qrc

greaterThan(QT_MAJOR_VERSION, 4): RC_FILE = ico.rc

QMAKE_CXXFLAGS += -std=c++11
