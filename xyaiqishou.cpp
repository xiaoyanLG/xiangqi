#include "xyaiqishou.h"
#include "xyqishou.h"
#include <QDebug>
#include "wushi.h"

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
    clear();
}

XYAIQishou::~XYAIQishou()
{

}

void XYAIQishou::setQipan(XYQipanWidget *qipan)
{
    this->qipan = qipan;
}

void XYAIQishou::clear()
{
    curMovedQizi = NULL;
    curQipanStatus = NULL;
    lastMovedQizi = NULL;
    lastQipanStatus = NULL;

    allMovedQizis.clear();
    qDeleteAll(allStatus);
    allStatus.clear();
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
    lastMovedQizi = curMovedQizi;
    curMovedQizi = qizi;
    lastQipanStatus = curQipanStatus;
    curQipanStatus = status;

    allMovedQizis.append(qizi);
    allStatus.append(status);

    if (XYQishou::getInstance()->getSideType() == qizi->getSideType())
    {
        start();
    }
}

void XYAIQishou::qiziMoved(const QPoint &src, const QPoint &tar, XYQiziWidget *qizi)
{
    if (XYQishou::getInstance()->getSideType() == qizi->getSideType())
    {
        bool win = wushiMoveQizi(src);
        if (win)
        {
            emit isWin(true);
            return;
        }
        targetPoint = tar;
        start();
    }
}

void XYAIQishou::getValues(int level,
                           XYQiziWidget::SIDETYPE type,
                           QList<MOVESTATUS *> &allBestPoints,
                           bool root, int &count)
{
    if (level <= 0)
    {
        return;
    }
    QList<MOVESTATUS *> curAllBestPoints;

    for (int i = 0; i < allBestPoints.size(); ++i)
    {
        XYQipanStatus *temp_curQipanStatus = allBestPoints.at(i)->qipanStatus;
        QPoint temp_point = allBestPoints.at(i)->point;
        XYQipanStatus::TYPE temp_type = allBestPoints.at(i)->type;
        const QMap<XYQipanStatus::TYPE, QList<QPoint> > &allPoints
                = temp_curQipanStatus->getAllQiziMovablePoints(type);


        int hong_value = 0;
        int hei_value = 0;
        QMap<XYQipanStatus::TYPE, QList<QPoint> >::const_iterator it = allPoints.begin();

        while (it != allPoints.end())
        {
            const QList<QPoint> &movablePoints = it.value();
            for (int j = 0; j < movablePoints.size(); ++j)
            {
                count++;
                XYQipanStatus *tempStatus = temp_curQipanStatus->moveQizi((XYQipanStatus::TYPE)it.key(),
                                                                     movablePoints.at(j).x(),
                                                                     movablePoints.at(j).y());

                tempStatus->getTotalAIValueOfStatus(hong_value, hei_value);
                if (type == XYQiziWidget::RED)
                {
                    int cur_value = hong_value - hei_value;

                    MOVESTATUS *thisIsMax = NULL;
                    if (root)
                    {
                        thisIsMax = new MOVESTATUS(movablePoints.at(j),
                                                   it.key(),
                                                   tempStatus,
                                                   cur_value);
                    }
                    else
                    {
                        thisIsMax = new MOVESTATUS(temp_point,
                                                   temp_type,
                                                   tempStatus,
                                                   cur_value);
                    }
                    curAllBestPoints.append(thisIsMax);
                }
                else
                {
                    int cur_value = hei_value - hong_value;
                    MOVESTATUS *thisIsMax = NULL;
                    if (root)
                    {
                        thisIsMax = new MOVESTATUS(movablePoints.at(j),
                                                   it.key(),
                                                   tempStatus,
                                                   cur_value);
                    }
                    else
                    {
                        thisIsMax = new MOVESTATUS(temp_point,
                                                   temp_type,
                                                   tempStatus,
                                                   cur_value);
                    }
                    curAllBestPoints.append(thisIsMax);
                }
            }
            ++it;
        }
    }

    level--;
    if (!root)
    {
        qDeleteAll(allBestPoints);
    }
    allBestPoints = curAllBestPoints;

    getValues(level, type, allBestPoints, false, count);
}

void XYAIQishou::run()
{
    wushiMoveQizi(targetPoint);
    QPoint src, tar;
    bool win = getPos(src, tar);

    XYQiziWidget *qizi = qipan->findQizi(src);
    if (qizi != NULL)
    {
        emit moveQizi(qizi, tar, false);
    }

    if (win)
    {
        emit isWin(false);
        return;
    }
}

