#include "xyaiqishou.h"
#include <QDebug>

XYAIQishou *XYAIQishou::instance = NULL;
XYAIQishou *XYAIQishou::getInstance()
{
    if (instance == NULL)
    {
        instance = new XYAIQishou;
    }
    return instance;
}

XYAIQishou::XYAIQishou(QObject *parent)
    : QThread(parent),
      level(3),
      aiType(AUTO),
      sideType(XYQiziWidget::UNKNOWN),
      lastQipanStatus(NULL)
{

}

XYAIQishou::~XYAIQishou()
{

}

void XYAIQishou::setQipan(XYQipanWidget *qipan)
{
    this->qipan = qipan;
}

void XYAIQishou::setLevel(int level)
{
    this->level = level;
}

void XYAIQishou::setAiType(XYAIQishou::AITYPE type)
{
    this->aiType = type;
}

void XYAIQishou::setSideType(XYQiziWidget::SIDETYPE type)
{
    this->sideType = type;
}

void XYAIQishou::qiziMoved(XYQiziWidget *qizi)
{
    XYQipanStatus *status = qipan->getCurQipanStatus();

    if (lastQipanStatus != NULL)
    {
//        qDebug() << "--------------------------------------------------";
//        qDebug() << QStringLiteral("红棋增加的点：") << status->getAddedPoints(lastQipanStatus, XYQiziWidget::RED);
//        qDebug() << QStringLiteral("红棋减少的点：") << status->getReducedPoints(lastQipanStatus, XYQiziWidget::RED);
//        qDebug() << QStringLiteral("黑棋增加的点：") << status->getAddedPoints(lastQipanStatus, XYQiziWidget::BLACK);
//        qDebug() << QStringLiteral("黑棋减少的点：") << status->getReducedPoints(lastQipanStatus, XYQiziWidget::BLACK);
    }
    lastQipanStatus = status;
}

