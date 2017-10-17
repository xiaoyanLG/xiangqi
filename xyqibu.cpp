#include "xyqibu.h"
#include "xyqipanwidget.h"

XYQibu::XYQibu(XYQiziWidget *target, const QPoint &pos, XYQiziWidget *eatenQizi)
    :target(target), curPos(pos)
{
    setEatenQizi(eatenQizi);
}

XYQibu::~XYQibu()
{

}

void XYQibu::setTarget(XYQiziWidget *target)
{
    this->target = target;
}

void XYQibu::setCurPos(const QPoint &pos)
{
    this->curPos = pos;
}

void XYQibu::setEatenQizi(XYQiziWidget *eatenQizi)
{
    this->eatenQizi = eatenQizi;
    if (eatenQizi != NULL)
    {
        this->eatenPos = eatenQizi->getCurPos();
    }
}

void XYQibu::switchViews()
{
    int x = 9 - curPos.x();
    int y = 8 - curPos.y();
    curPos = QPoint(x, y);

    x = 9 - eatenPos.x();
    y = 8 - eatenPos.y();
    eatenPos = QPoint(x, y);
}

