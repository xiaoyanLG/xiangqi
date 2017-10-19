#include "mainwindow.h"
#include "xyinput.h"
#include <QApplication>

// 指定qDebug输出执行函数
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static FILE *file = fopen("I:/DeskTop/log.txt", "w+");
    if (file == NULL) {
        FILE *file = fopen("I:/DeskTop/log.txt", "w+");
        if (file == NULL) {
            printf("Warning: Open log file failed!\n");
            file = stdout;
        }
    }
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(file, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fflush(file);
        break;
    case QtWarningMsg:
        fprintf(file, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fflush(file);
        break;
    case QtCriticalMsg:
        fprintf(file, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fflush(file);
        break;
    case QtFatalMsg:
        fprintf(file, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fflush(file);
        abort();
    }
}

int main(int argc, char *argv[])
{
//    qInstallMessageHandler(myMessageOutput); //安装LOG

    QApplication a(argc, argv);
    XYInput::getInstance()->initInputBase(QApplication::applicationDirPath() + "/chineseBase/chinese.db");
    MainWindow w;
    w.show();
//    w.showFullScreen();

    return a.exec();
}
