#include "xybattleinfowidget.h"
#include "mainwindow.h"
#include "xyqipanwidget.h"
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>

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

    allOnlinePeoplesWidget = new QListWidget;
    allOnlinePeoplesWidget->setFixedWidth(150);
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
    QPushButton *switchBtn = new QPushButton(QString::fromStdWString(L"切换视角"));
    switchBtn->setFixedSize(150, 30);
    connect(switchBtn, SIGNAL(clicked()), this, SLOT(switchViews()));

    sendMessageEdit = new QLineEdit;
    sendMessageEdit->setFixedWidth(150);
    connect(sendMessageEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    messageBox = new QTextEdit;
    messageBox->setFixedWidth(150);

    layout->setContentsMargins(25, 20, 0, 30);

    layout->addWidget(allOnlinePeoplesWidget, 1);
    layout->addWidget(switchBtn);
    layout->addWidget(revokedBtn);
    layout->addWidget(layoutBtn);
    layout->addWidget(ZoomOutBtn);
    layout->addWidget(ZoomInBtn);
    layout->addWidget(closeBtn);
    layout->addWidget(messageBox, 1);
    layout->addWidget(sendMessageEdit);

    qipanPixmap.load(":/xiangqi/qipan.png");

    allOnlinePeoplesWidget->addItem(XYUdpbroadcast::getUserName());
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

void XYBattleInfoWidget::peopleUpline(const QString &name, const QHostAddress &address)
{
    // 判断是否重复
    QMap<int, QHostAddress>::iterator it = allPeoplesMap.begin();

    while (it != allPeoplesMap.end())
    {
        const QHostAddress &address_in = it.value();
        if (address_in == address)
        {
            return;
        }
        ++it;
    }

    QListWidgetItem *item = new QListWidgetItem(name);
    allOnlinePeoplesWidget->addItem(item);
    allPeoplesMap.insert(allOnlinePeoplesWidget->row(item), address);
}

void XYBattleInfoWidget::peopleOffline(const QString &name, const QHostAddress &address)
{
    QMap<int, QHostAddress>::iterator it = allPeoplesMap.begin();

    while (it != allPeoplesMap.end())
    {
        const QHostAddress &address_in = it.value();
        if (address_in == address)
        {
            allOnlinePeoplesWidget->takeItem(it.key());
        }
        ++it;
    }
}

void XYBattleInfoWidget::receiveData(const QString &from, const QString &data)
{
    messageBox->setTextColor("blue");
    messageBox->append(from + ": " + data);
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

void XYBattleInfoWidget::switchViews()
{
    MainWindow::getInstance()->switchViews();
}

void XYBattleInfoWidget::sendMessage()
{
    QString data = sendMessageEdit->text();
    QHostAddress address = allPeoplesMap.value(allOnlinePeoplesWidget->currentRow());
    if (address.isNull())
    {
        address = QHostAddress(QHostAddress::Broadcast);
    }

    messageBox->setTextColor("green");
    messageBox->append(QStringLiteral("你说") + ": " + data);
    emit sendMessage(address, data);
}
