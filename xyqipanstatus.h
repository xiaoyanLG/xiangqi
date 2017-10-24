#ifndef XYQIPANSTATUS_H
#define XYQIPANSTATUS_H

#include <QPoint>
#include <QList>
#include <QMap>
#include "xyqiziwidget.h"
#include "xyqipanwidget.h"

class XYQipanStatus
{
public:
    enum TYPE{HONG_ZU1 = 1, HONG_ZU2, HONG_ZU3, HONG_ZU4, HONG_ZU5,
              HONG_PAO1, HONG_PAO2, HONG_CHE1, HONG_CHE2, HONG_MA1, HONG_MA2,
              HONG_XIANG1, HONG_XIANG2, HONG_SI1, HONG_SI2, HONG_JIANG = 16,
              HEI_ZU1 = 17, HEI_ZU2, HEI_ZU3, HEI_ZU4, HEI_ZU5,
              HEI_PAO1, HEI_PAO2, HEI_CHE1, HEI_CHE2, HEI_MA1, HEI_MA2,
              HEI_XIANG1, HEI_XIANG2, HEI_SI1, HEI_SI2, HEI_JIANG = 32};

    explicit XYQipanStatus(XYQipanWidget *qipanWidget);
    ~XYQipanStatus();
    bool isOver();
    void switchViews();
    XYQiziWidget *getQiziWithType(TYPE type);
    int getQiziAIValue(TYPE type, int row, int column, bool hong);
    void getTotalAIValueOfStatus(int &hong, int &hei);

    XYQipanStatus *moveQizi(TYPE qizi, int r, int c);
    QMap<TYPE, QList<QPoint> > &getAllQiziMovablePoints(XYQiziWidget::SIDETYPE type);
    QList<QPoint> getQiziMovablePoints(TYPE type);
    QList<QPoint> getQiziMovablePoints(TYPE type, int row, int column);

    QMap<TYPE, QList<QPoint> > getAddedPoints(XYQipanStatus *other, XYQiziWidget::SIDETYPE type);
    QMap<TYPE, QList<QPoint> > getReducedPoints(XYQipanStatus *other, XYQiziWidget::SIDETYPE type);

    TYPE qipan[10][9];
    bool hongIsSmall;
    XYQipanWidget *qipanWidget;   // 记录当前棋盘状态所属的棋盘

    QMap<TYPE, QList<QPoint> > allHongMovablePoints;
    QMap<TYPE, QList<QPoint> > allHeiMovablePoints;
    QMap<TYPE, QPoint > allHongEatenPoints;
    QMap<TYPE, QPoint > allHeiEatenPoints;
};

#endif // XYQIPANSTATUS_H
