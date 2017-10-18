#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xyqiziwidget.h"
#include <QMouseEvent>
#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QDebug>

MainWindow *MainWindow::instance;
MainWindow *MainWindow::getInstance()
{
    return instance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalLayout_2->setContentsMargins(0, 0, 0, 0);

    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowSystemMenuHint
                   | Qt::WindowMinimizeButtonHint);

    // 创建棋手
    me = XYQishou::getInstance();
    me->setParent(this);
    me->setQipan(ui->widget);

    ai = new XYAIQishou(this);
    ai->setQipan(ui->widget);
    connect(ui->widget, SIGNAL(qiziMoved(XYQiziWidget*,QPoint)),
            ai, SLOT(qiziMoved(XYQiziWidget*,QPoint)));

    connect(me, SIGNAL(peopleUpline(QString,QHostAddress)),
            ui->widget_2, SLOT(peopleUpline(QString,QHostAddress)));
    connect(me, SIGNAL(peopleOffline(QString,QHostAddress)),
            ui->widget_2, SLOT(peopleOffline(QString,QHostAddress)));
    connect(me, SIGNAL(receiveMessage(QString,QString)),
            ui->widget_2, SLOT(receiveData(QString,QString)));
    connect(me, SIGNAL(sideChanged()),
            ui->widget_2, SLOT(qishouSideChanged()));
    connect(me, SIGNAL(moveQizi(XYQiziWidget*,QPoint,bool)), ui->widget,
            SLOT(moveQizi(XYQiziWidget*,QPoint,bool)));

    connect(ui->widget_2, SIGNAL(sendMessage(QHostAddress,QString)),
            me, SLOT(sendMessage(QHostAddress,QString)));

    connect(me, SIGNAL(showMessages(QString, int)), this, SLOT(showMessage(QString, int)));
    connect(ui->widget, SIGNAL(showMessages(QString)), this, SLOT(showMessage(QString)));

    instance = this;
    resize(1000, 800);
    layoutQizi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessage(const QString &msg, int times)
{
    static QLabel *msgLabel = NULL;
    static QTimer *timer = NULL;
    if (msgLabel == NULL)
    {
        msgLabel = new QLabel(this);
        msgLabel->setAlignment(Qt::AlignCenter);
        timer = new QTimer;
        connect(timer, SIGNAL(timeout()), msgLabel, SLOT(close()));
        QFont font = msgLabel->font();
        font.setFamily(QString::fromStdWString(L"华文行楷"));
        font.setPointSize(25 * height() / 800);
        msgLabel->setFont(font);
        msgLabel->setStyleSheet("background-color: rgb(0, 0, 0, 0); "
                                "color: rgb(248, 237, 56)");
    }
    QFont font = msgLabel->font();
    font.setPointSize(25 * height() / 800);
    msgLabel->setFont(font);
    msgLabel->raise();
    msgLabel->setText(msg);
    msgLabel->resize(width() - 200, 50);
    if (times == 0)
    {
        timer->stop();
    }
    else
    {
        timer->start(times);
    }
    msgLabel->show();
    msgLabel->move(0, (height() - msgLabel->height()) / 2);
}

void MainWindow::testsssss()
{
//    static int x = 0;
//    static int y = 0;
//    qDebug() << ui->widget->size();
//    hong_qizis.at(0)->setVisible(true);
//    hong_qizis.at(0)->move(ui->widget->allPos[x][y] -
//            QPoint(hong_qizis.at(0)->width() / 2, hong_qizis.at(0)->height() / 2));

//    qDebug() << QPoint(x, y) << ui->widget->allPos[x][y];
//    y++;
//    if (y == 9)
//    {
//        x++;
//        y = 0;
//        if (x == 10)
//        {
//            x = 0;
//        }
//    }
}

void MainWindow::layoutQizi(bool keep)
{
    ui->widget->layoutQizi(keep);
}

void MainWindow::switchViews()
{
    ui->widget->switchViews();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = true;
        moLastPos = event->globalPos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = false;
        moLastPos = event->globalPos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mbLeftMousePressed)
    {
        QPoint lastpos(x(), y());
        lastpos.setX( lastpos.x() + event->globalX() - moLastPos.x());
        lastpos.setY( lastpos.y() + event->globalY() - moLastPos.y());
        move(lastpos);
        moLastPos = event->globalPos();
    }
}
