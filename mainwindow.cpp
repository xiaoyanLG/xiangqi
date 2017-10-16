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

    for (int i = XYQiziWidget::HONG_ZU; i <= XYQiziWidget::HONG_JIANG; ++i)
    {
        int times = 0;
        XYQiziWidget::TYPE type = (XYQiziWidget::TYPE)i;
        switch (type)
        {
        case XYQiziWidget::HONG_ZU:
            times = 5;
            break;
        case XYQiziWidget::HONG_JIANG:
            times = 1;
            break;
        default:
            times = 2;
            break;
        }

        for (int j = 0; j < times; ++j)
        {
            XYQiziWidget *qizi = new XYQiziWidget(type, j, this);
            hong_qizis.append(qizi);
            connect(ui->widget, SIGNAL(sizeChanged(QSize)), qizi, SLOT(resizeQizi(QSize)));
        }
    }
    for (int i = XYQiziWidget::HEI_ZU; i <= XYQiziWidget::HEI_JIANG; ++i)
    {
        int times = 0;
        XYQiziWidget::TYPE type = (XYQiziWidget::TYPE)i;
        switch (type)
        {
        case XYQiziWidget::HEI_ZU:
            times = 5;
            break;
        case XYQiziWidget::HEI_JIANG:
            times = 1;
            break;
        default:
            times = 2;
            break;
        }

        for (int j = 0; j < times; ++j)
        {
            XYQiziWidget *qizi = new XYQiziWidget(type, j, this);
            hei_qizis.append(qizi);
            connect(ui->widget, SIGNAL(sizeChanged(QSize)), qizi, SLOT(resizeQizi(QSize)));
        }
    }

    // 准备临时棋子
    tempQizi = new XYQiziWidget(XYQiziWidget::TEMP, 0, this);
    connect(ui->widget, SIGNAL(sizeChanged(QSize)), tempQizi, SLOT(resizeQizi(QSize)));
    ui->widget->setTempQizi(tempQizi);

    // 创建棋手
    me = XYQishou::getInstance();
    me->setParent(this);
    me->setQizi(hong_qizis, XYQiziWidget::RED);
    me->setQizi(hei_qizis, XYQiziWidget::BLACK);

    connect(me, SIGNAL(peopleUpline(QString,QHostAddress)),
            ui->widget_2, SLOT(peopleUpline(QString,QHostAddress)));
    connect(me, SIGNAL(peopleOffline(QString,QHostAddress)),
            ui->widget_2, SLOT(peopleOffline(QString,QHostAddress)));
    connect(me, SIGNAL(receiveMessage(QString,QString)),
            ui->widget_2, SLOT(receiveData(QString,QString)));
    connect(me, SIGNAL(moveQizi(XYQiziWidget*,QPoint,bool)), ui->widget,
            SLOT(moveQizi(XYQiziWidget*,QPoint,bool)));

    connect(ui->widget_2, SIGNAL(sendMessage(QHostAddress,QString)),
            me, SLOT(sendMessage(QHostAddress,QString)));

    connect(ui->widget, SIGNAL(showMessages(QString)), this, SLOT(showMessage(QString)));

    instance = this;
    resize(1000, 800);
    layoutQizi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessage(const QString &msg)
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
        font.setPointSize(25);
        msgLabel->setFont(font);
        msgLabel->setStyleSheet("background-color: rgb(0, 0, 0, 0); "
                                "color: rgb(248, 237, 56)");
    }
    msgLabel->raise();
    msgLabel->setText(msg);
    msgLabel->resize(width() - 200, 50);
    timer->start(3000);
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

void MainWindow::layoutQizi()
{
    static bool up = true;
    ui->widget->clear(true);
    for (int i = 0; i < hong_qizis.size(); ++i)
    {
        hong_qizis.at(i)->setVisible(true);
        ui->widget->putQiziToDefaultPos(hong_qizis.at(i), up);
    }

    for (int i = 0; i < hei_qizis.size(); ++i)
    {
        hei_qizis.at(i)->setVisible(true);
        ui->widget->putQiziToDefaultPos(hei_qizis.at(i), !up);
    }
    up = !up;
}

void MainWindow::switchViews()
{
    ui->widget->clear(false);
    for (int i = 0; i < hong_qizis.size(); ++i)
    {
        XYQiziWidget *qizi = hong_qizis.at(i);
        qizi->switchViews();
        if (!qizi->getBeEaten())
        {
            ui->widget->putQizi(qizi, qizi->getCurPos().x(), qizi->getCurPos().y(), false);
        }
    }

    for (int i = 0; i < hei_qizis.size(); ++i)
    {
        XYQiziWidget *qizi = hei_qizis.at(i);
        qizi->switchViews();
        if (!qizi->getBeEaten())
        {
            ui->widget->putQizi(qizi, qizi->getCurPos().x(), qizi->getCurPos().y(), false);
        }
    }
    ui->widget->switchViews();
}

void MainWindow::appendMessage(const QString &from, const QString &message)
{
    qDebug() << __FUNCTION__;
}

void MainWindow::newParticipant(const QString &nick)
{
    qDebug() << __FUNCTION__;
}

void MainWindow::participantLeft(const QString &nick)
{
    qDebug() << __FUNCTION__;
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
