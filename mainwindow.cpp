#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xyqiziwidget.h"
#include <QMouseEvent>
#include <QApplication>
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

    instance = this;
    resize(1000, 800);
    layoutQizi();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    for (int i = 0; i < hong_qizis.size(); ++i)
    {
        QPoint pos = hong_qizis.at(i)->getQiziDefaultPos(up);
        hong_qizis.at(i)->setVisible(true);
        ui->widget->putQizi(hong_qizis.at(i), pos.x(), pos.y());
    }

    for (int i = 0; i < hei_qizis.size(); ++i)
    {
        QPoint pos = hei_qizis.at(i)->getQiziDefaultPos(!up);
        hei_qizis.at(i)->setVisible(true);
        ui->widget->putQizi(hei_qizis.at(i), pos.x(), pos.y());
    }
    up = !up;
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
