#ifndef XYQIBU_H
#define XYQIBU_H

#include "xyqiziwidget.h"

class XYQibu
{
public:
    explicit XYQibu(XYQiziWidget *target = NULL,
                    const QPoint &pos = QPoint(-1, -1),
                    XYQiziWidget *eatenQizi = NULL);
    ~XYQibu();

    void setTarget(XYQiziWidget *target);
    void setCurPos(const QPoint &pos);
    void setEatenQizi(XYQiziWidget *eatenQizi);
    void switchViews();

private:
    XYQiziWidget *target;
    QPoint        curPos;
    XYQiziWidget *eatenQizi;
    QPoint        eatenPos;

    friend class XYQipanWidget;
};

#endif // XYQIBU_H
