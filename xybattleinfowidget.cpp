#include "xybattleinfowidget.h"
#include "mainwindow.h"
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

XYBattleInfoWidget::XYBattleInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(1);
    QPushButton *closeBtn = new QPushButton("Close");
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(exit()));
    QPushButton *ZoomInBtn = new QPushButton("Zoom In");
    connect(ZoomInBtn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    QPushButton *ZoomOutBtn = new QPushButton("Zoom Out");
    connect(ZoomOutBtn, SIGNAL(clicked()), this, SLOT(zoomOut()));
    QPushButton *layoutBtn = new QPushButton("Layout Qizi");
    connect(layoutBtn, SIGNAL(clicked()), this, SLOT(layoutQizi()));

    layout->addWidget(layoutBtn);
    layout->addWidget(ZoomOutBtn);
    layout->addWidget(ZoomInBtn);
    layout->addWidget(closeBtn);
}

XYBattleInfoWidget::~XYBattleInfoWidget()
{

}

void XYBattleInfoWidget::paintEvent(QPaintEvent *event)
{

}

void XYBattleInfoWidget::exit()
{
    if (QMessageBox::question(this->parentWidget(),
                              "Warning",
                              "Are you sure to quit?",
                              QMessageBox::Yes|QMessageBox::No)
            == QMessageBox::Yes)
    {
        MainWindow::getInstance()->close();
    }
}

void XYBattleInfoWidget::zoomIn()
{
    MainWindow::getInstance()->resize(MainWindow::getInstance()->size() + QSize(50, 55));
}

void XYBattleInfoWidget::zoomOut()
{
    MainWindow::getInstance()->testsssss();
    //    MainWindow::getInstance()->resize(MainWindow::getInstance()->size() - QSize(50, 55));
}

void XYBattleInfoWidget::layoutQizi()
{
    MainWindow::getInstance()->layoutQizi();
}

