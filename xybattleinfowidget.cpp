#include "xybattleinfowidget.h"
#include "mainwindow.h"
#include "xyqipanwidget.h"
#include "xyinput.h"
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>

XYBattleInfoWidget *XYBattleInfoWidget::instance = NULL;
XYBattleInfoWidget *XYBattleInfoWidget::getInstance()
{
    return instance;
}

XYBattleInfoWidget::XYBattleInfoWidget(QWidget *parent)
    : QWidget(parent)
{
    instance = this;
#if QT_VERSION >=0x050000
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
#endif

    int w = 130;
    int h = 28;
    allOnlinePeoplesWidget = new QListWidget;
    allOnlinePeoplesWidget->setFixedWidth(w);
    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *closeBtn = new QPushButton(QString::fromStdWString(L"退出"));
    closeBtn->setFixedSize(w, h);
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(exit()));
    QPushButton *ZoomInBtn = new QPushButton(QString::fromStdWString(L"放大"));
    ZoomInBtn->setFixedSize(w, h);
    connect(ZoomInBtn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    QPushButton *ZoomOutBtn = new QPushButton(QString::fromStdWString(L"缩小"));
    ZoomOutBtn->setFixedSize(w, h);
    connect(ZoomOutBtn, SIGNAL(clicked()), this, SLOT(zoomOut()));
    QPushButton *layoutBtn = new QPushButton(QString::fromStdWString(L"重新布棋"));
    layoutBtn->setFixedSize(w, h);
    connect(layoutBtn, SIGNAL(clicked()), this, SLOT(layoutQizi()));
    QPushButton *revokedBtn = new QPushButton(QString::fromStdWString(L"悔棋"));
    revokedBtn->setFixedSize(w, h);
    connect(revokedBtn, SIGNAL(clicked()), this, SLOT(revoked()));
    QPushButton *switchBtn = new QPushButton(QString::fromStdWString(L"切换视角"));
    switchBtn->setFixedSize(w, h);
    connect(switchBtn, SIGNAL(clicked()), this, SLOT(switchViews()));
    QPushButton *switchColorBtn = new QPushButton(QString::fromStdWString(L"切换棋方"));
    switchColorBtn->setFixedSize(w, h);
    switchColorBtn->setIcon(XYQishou::getInstance()->getSideIcon());
    connect(switchColorBtn, SIGNAL(clicked()), this, SLOT(switchColor()));
    QPushButton *hostingBtn = new QPushButton(QString::fromStdWString(L"托管"));
    hostingBtn->setFixedSize(w, h);
    connect(hostingBtn, SIGNAL(clicked()), this, SLOT(hosting()));
    QPushButton *receiveBtn = new QPushButton(QString::fromStdWString(L"接收广播棋步"));
    receiveBtn->setFixedSize(w, h);
    connect(receiveBtn, SIGNAL(clicked()), this, SLOT(receiveUDP()));
    QPushButton *sendBtn = new QPushButton(QString::fromStdWString(L"发送广播棋步"));
    sendBtn->setFixedSize(w, h);
    connect(sendBtn, SIGNAL(clicked()), this, SLOT(sendUDP()));
    QPushButton *aiBtn = new QPushButton(QString::fromStdWString(L"开启AI"));
    aiBtn->setFixedSize(w, h);
    connect(aiBtn, SIGNAL(clicked()), this, SLOT(startAI()));

    sendMessageEdit = new QLineEdit;
    sendMessageEdit->installEventFilter(XYInput::getInstance());
    sendMessageEdit->setFixedWidth(w);
    connect(sendMessageEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    messageBox = new QTextEdit;
    messageBox->setFixedWidth(w);

    layout->setContentsMargins(15, 20, 0, 30);

    layout->addWidget(allOnlinePeoplesWidget, 1);
    layout->addWidget(receiveBtn);
    layout->addWidget(sendBtn);
    layout->addWidget(aiBtn);
    layout->addWidget(hostingBtn);
    layout->addWidget(switchColorBtn);
    layout->addWidget(switchBtn);
    layout->addWidget(revokedBtn);
    layout->addWidget(layoutBtn);
    layout->addWidget(ZoomOutBtn);
    layout->addWidget(ZoomInBtn);
    layout->addWidget(closeBtn);
    layout->addWidget(messageBox, 1);
    layout->addWidget(sendMessageEdit);

    qipanPixmap.load(":/xiangqi/qipan.png");

    funcBtns.insert(0, switchColorBtn);
    funcBtns.insert(1, hostingBtn);

    allOnlinePeoplesWidget->addItem(XYUdpbroadcast::getUserName());

    connect(allOnlinePeoplesWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(connectPeople(QListWidgetItem*)));
}

XYBattleInfoWidget::~XYBattleInfoWidget()
{

}

QHostAddress XYBattleInfoWidget::getSendHostAddress()
{
    QHostAddress address = allPeoplesMap.value(allOnlinePeoplesWidget->currentRow());
    if (address.isNull())
    {
        address = QHostAddress(QHostAddress::Broadcast);
    }

    return address;
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
            allPeoplesMap.erase(it);
            break;
        }
        ++it;
    }
}

void XYBattleInfoWidget::receiveData(const QString &from, const QString &data)
{
    messageBox->setTextColor("blue");
    messageBox->append(from + ": " + data);
}

void XYBattleInfoWidget::qishouSideChanged()
{
    QPushButton *btn = funcBtns.value(0);
    btn->setIcon(XYQishou::getInstance()->getSideIcon());
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

void XYBattleInfoWidget::switchColor()
{
    QPushButton *btn = (QPushButton *)sender();
    XYQiziWidget::SIDETYPE type = XYQishou::getInstance()->switchSideType();
    btn->setIcon(XYQishou::getInstance()->getSideIcon());

    if (XYQishou::getInstance()->isHosting())
    {
        funcBtns.value(1)->setIcon(QIcon());
        XYQishou::getInstance()->setHosting(false);
    }

    if (type == XYQiziWidget::RED)
    {
        XYAIQishou::getInstance()->setSideType(XYQiziWidget::BLACK);
    }
    else if (type == XYQiziWidget::BLACK)
    {
        XYAIQishou::getInstance()->setSideType(XYQiziWidget::RED);
    }
}

void XYBattleInfoWidget::hosting()
{
    QPushButton *btn = (QPushButton *)sender();
    if (!XYQishou::getInstance()->isHosting())
    {
        btn->setIcon(XYQishou::getInstance()->getSideIcon());
        XYQishou::getInstance()->setHosting(true);
#if QT_VERSION >=0x050000
        btn->setStyleSheet("QPushButton {\
                           background-color: #2d9d00;\
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
                       }");
#endif
    }
    else
    {
        btn->setIcon(QIcon());
        XYQishou::getInstance()->setHosting(false);
#if QT_VERSION >=0x050000
        btn->setStyleSheet("QPushButton {\
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
                       }");
#endif
    }
}

void XYBattleInfoWidget::receiveUDP()
{
    QPushButton *btn = (QPushButton *)sender();
    XYQishou::getInstance()->switchReceive();
    if (XYQishou::getInstance()->isReceiveUDPQibu())
    {
        btn->setIcon(QIcon(":/other/used.ico"));
#if QT_VERSION >=0x050000
        btn->setStyleSheet("QPushButton {\
                           background-color: #2d9d00;\
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
                       }");
#endif
    }
    else
    {
        btn->setIcon(QIcon());
#if QT_VERSION >=0x050000
        btn->setStyleSheet("QPushButton {\
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
                       }");
#endif
    }
}

void XYBattleInfoWidget::sendUDP()
{

    QPushButton *btn = (QPushButton *)sender();
    XYQishou::getInstance()->switchSend();
    if (XYQishou::getInstance()->isSendUDPQibu())
    {
        btn->setIcon(QIcon(":/other/used.ico"));
#if QT_VERSION >=0x050000
        btn->setStyleSheet("QPushButton {\
                           background-color: #2d9d00;\
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
                       }");
#endif
    }
    else
    {
        btn->setIcon(QIcon());
#if QT_VERSION >=0x050000
        btn->setStyleSheet("QPushButton {\
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
                       }");
#endif
    }
}

void XYBattleInfoWidget::startAI()
{
    QPushButton *btn = (QPushButton *)sender();
    XYAIQishou::getInstance()->switchAI();
    if (XYAIQishou::getInstance()->isAIStart())
    {
        btn->setIcon(QIcon(":/other/used.ico"));
#if QT_VERSION >=0x050000
        btn->setStyleSheet("QPushButton {\
                           background-color: #2d9d00;\
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
                       }");
#endif
    }
    else
    {
        btn->setIcon(QIcon());
#if QT_VERSION >=0x050000
        btn->setStyleSheet("QPushButton {\
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
                       }");
#endif
    }
}

void XYBattleInfoWidget::sendMessage()
{
    QString data = sendMessageEdit->text();
    if (data.isEmpty())
    {
        return;
    }
    sendMessageEdit->clear();
    QHostAddress address = getSendHostAddress();

    messageBox->setTextColor("green");
    messageBox->append(QString::fromStdWString(L"你说: ") + data);
    emit sendMessage(address, data);
}

void XYBattleInfoWidget::connectPeople(QListWidgetItem *item)
{
    QHostAddress address = allPeoplesMap.value(allOnlinePeoplesWidget->row(item));
    XYQishou::getInstance()->connectPeople(address);
}
