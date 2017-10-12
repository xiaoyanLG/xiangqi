#include "xybattleinfowidget.h"
#include "mainwindow.h"
#include "xyqipanwidget.h"
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>

XYBattleInfoWidget::XYBattleInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    QString qss0="QPushButton {\
                    background-color: #473C8B;\
                    border-style: outset;\
                    border-width: 2px;\
                    border-radius: 5px;\
                    border-color: #8B7355;\
                    font: bold 14px;\
                    min-width:2em;\
                    color:white;\
                    font-family:华文新魏;\
                    padding: 5px;\
                }\
                QPushButton:pressed {\
                    background-color: #1E90FF;\
                    border-style: inset;\
                }";
    this->setStyleSheet(qss0);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *closeBtn = new QPushButton(QString::fromStdWString(L"退出"));
    closeBtn->setFixedSize(150, 30);
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(exit()));
    QPushButton *ZoomInBtn = new QPushButton(QString::fromStdWString(L"放大"));
    ZoomInBtn->setFixedSize(150, 30);
    connect(ZoomInBtn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    QPushButton *ZoomOutBtn = new QPushButton(QString::fromStdWString(L"缩小"));
    ZoomOutBtn->setFixedSize(150, 30);
    connect(ZoomOutBtn, SIGNAL(clicked()), this, SLOT(zoomOut()));
    QPushButton *layoutBtn = new QPushButton(QString::fromStdWString(L"布棋"));
    layoutBtn->setFixedSize(150, 30);
    connect(layoutBtn, SIGNAL(clicked()), this, SLOT(layoutQizi()));
    QPushButton *revokedBtn = new QPushButton(QString::fromStdWString(L"悔棋"));
    revokedBtn->setFixedSize(150, 30);
    connect(revokedBtn, SIGNAL(clicked()), this, SLOT(revoked()));

    layout->addStretch();
    layout->setContentsMargins(25, 0, 0, 0);
    layout->addWidget(revokedBtn);
    layout->addWidget(layoutBtn);
    layout->addWidget(ZoomOutBtn);
    layout->addWidget(ZoomInBtn);
    layout->addWidget(closeBtn);
    layout->addStretch();

    qipanPixmap.load(":/xiangqi/qipan.png");
}

XYBattleInfoWidget::~XYBattleInfoWidget()
{

}

void XYBattleInfoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing
                          | QPainter::TextAntialiasing
                          | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect(), qipanPixmap, QRect(10, 10,
                                                  qipanPixmap.width() - 20,
                                                  qipanPixmap.height() - 20));
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

void XYBattleInfoWidget::revoked()
{
    XYQipanWidget::getInstance()->revokeLastQibu();
}

