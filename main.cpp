#include "mainwindow.h"
#include "xyinput.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XYInput::getInstance()->initInputBase(QApplication::applicationDirPath() + "/chineseBase/chinese.db");
    MainWindow w;
    w.show();
//    w.showFullScreen();

    return a.exec();
}
