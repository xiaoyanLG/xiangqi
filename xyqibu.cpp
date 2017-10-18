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
    curPos = XYQiziWidget::getSwitchViewsPos(curPos);
    eatenPos = XYQiziWidget::getSwitchViewsPos(eatenPos);
}

