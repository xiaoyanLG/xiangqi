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
    QPushButton *closeBtn = new QPushButton(QString::fromStdWString(L"退出"));
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(exit()));
    QPushButton *ZoomInBtn = new QPushButton(QString::fromStdWString(L"放大"));
    connect(ZoomInBtn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    QPushButton *ZoomOutBtn = new QPushButton(QString::fromStdWString(L"缩小"));
    connect(ZoomOutBtn, SIGNAL(clicked()), this, SLOT(zoomOut()));
    QPushButton *layoutBtn = new QPushButton(QString::fromStdWString(L"布棋"));
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
                              QString::fromStdWString(L"警告"),
                              QString::fromStdWString(L"你确定退出吗?"),
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
    MainWindow::getInstance()->resize(MainWindow::getInstance()->size() - QSize(50, 55));
}

void XYBattleInfoWidget::layoutQizi()
{
    MainWindow::getInstance()->layoutQizi();
}

