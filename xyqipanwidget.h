#ifndef XYQIPANWIDGET_H
#define XYQIPANWIDGET_H

#include <QWidget>
#include <QStack>
#include "xyqiziwidget.h"
#include "xyqibu.h"
#include "xyqipanstatus.h"

class XYQipanWidget : public QWidget
{
    Q_OBJECT
public:
    static XYQipanWidget *getInstance();
    explicit XYQipanWidget(QWidget *parent = 0);
    ~XYQipanWidget();
    void clear(bool clearHistory);
    void putQiziToDefaultPos(XYQiziWidget *qizi, bool up);
    void putQizi(XYQiziWidget *qizi, int row, int column, bool addHistory, bool animation = false);
    void revokeQizi(XYQiziWidget *qizi, int row, int column, bool beEaten);
    void moveToNearestPos(XYQiziWidget *qizi);
    XYQiziWidget *getPositionQizi(int row, int column);
    void raiseTempQizi();
    void showTempQizi(XYQiziWidget *qizi);
    void revokeLastQibu(bool socket = true);
    XYQiziWidget *findQizi(const QPoint &point);
    XYQiziWidget *findQizi(const QByteArray &key,
                           XYQiziWidget::TYPE type,
                           int times,
                           const QPoint &lastPoint,
                           QPoint &movePoint);

    QByteArray getQipanStatus();
    XYQipanStatus *getCurQipanStatus();

signals:
    void qiziMoved(XYQiziWidget *qizi);
    void qiziMoved(const QPoint &src, const QPoint &tar, XYQiziWidget *qizi);
    void sizeChanged(const QSize &size);
    void showMessages(const QString &msg);

public slots:
    void layoutQizi(bool keep = false);
    void switchViews();
    void moveQizi(XYQiziWidget *qizi, const QPoint &point, bool revoked);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void initQizi();
    QPoint getQiziCurNearestPos(XYQiziWidget *qizi);
    QPainterPath getPosPath(const QPointF &point, qreal w, int type); // type: 1, 2, 1|2

private:
    static XYQipanWidget *instance;          // 棋盘单例
    XYQiziWidget         *hong_jiang;        // 记录红将的句柄，容易查找
    QList<XYQiziWidget *> hong_qizis;        // 所有的红棋子
    QList<XYQiziWidget *> hei_qizis;         // 所有的黑棋子
    XYQiziWidget         *tempQizi;          // 临时棋子，阴影显示当前可移动的棋步
    QPixmap               qipanPixmap;       // 棋盘背景图片
    QPoint                allPos[10][9];     // 记录棋盘上的可下棋的点
    XYQiziWidget         *qiziInqipan[10][9];// 记录棋盘当前棋子所在
    QStack<XYQibu *>      historyQibus;      // 记录历史的棋步
    int                   allMoveTimes;      // 记录开局以来移动的总次数

    XYQiziWidget::SIDETYPE lastSideType;     // 记录上次下棋的棋方

    friend class XYQipanStatus;
    friend class XYQishou;
    friend class XYAIQishou;
};

#endif // XYQIPANWIDGET_H
