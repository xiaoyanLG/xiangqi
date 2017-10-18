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
      sideType(XYQiziWidget::UNKNOWN)
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
    hongCurMovablePoints = qipan->getCurQiziMovablePoints(XYQiziWidget::RED);
    heiCurMovablePoints = qipan->getCurQiziMovablePoints(XYQiziWidget::BLACK);
    qDebug() << (XYQiziWidget::TYPE)qizi->type << qizi->moveTimes << qipan->allMoveTimes;
}

