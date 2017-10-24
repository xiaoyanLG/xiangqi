#ifndef XYAIQISHOU_H
#define XYAIQISHOU_H

#include <QThread>
#include "xyqipanwidget.h"

struct MOVESTATUS{
    MOVESTATUS(const QPoint &point,
               XYQipanStatus::TYPE type,
               XYQipanStatus *qipanStatus,
               int value)
        :point(point),type(type),qipanStatus(qipanStatus),value(value)
    {

    }
    ~MOVESTATUS()
    {
        delete qipanStatus;
    }

    int value;
    QPoint point;
    XYQipanStatus::TYPE type;
    XYQipanStatus *qipanStatus;
};

class XYAIQishou : public QThread
{
    Q_OBJECT
public:
    enum AITYPE{ATTACK, GUARD, AUTO};
    static XYAIQishou *getInstance();
    explicit XYAIQishou(QObject *parent = 0);
    ~XYAIQishou();
    void setQipan(XYQipanWidget *qipan);
    void clear();

signals:
    void isWin(bool win);
    void moveQizi(XYQiziWidget *qizi, const QPoint &point, bool revoked = false);

public slots:
    void setLevel(int level);
    void setAiType(AITYPE type);
    void setSideType(XYQiziWidget::SIDETYPE type);
    void qiziMoved(XYQiziWidget *qizi);

    // 象棋巫师接口
    void qiziMoved(const QPoint &src, const QPoint &tar, XYQiziWidget *qizi);

protected:
    void getValues(int level,
                   XYQiziWidget::SIDETYPE type,
                   QList<MOVESTATUS *> &allBestPoints,
                   bool root, int &count); // 递归函数，获取最好的下子位置
    void run();

private:
    static XYAIQishou     *instance;
    XYQipanWidget         *qipan;
    int                    level;       // 记录AI技术水平
    XYQiziWidget::SIDETYPE sideType;    // 记录AI走哪一方
    AITYPE                 aiType;      // 记录AI类型（进攻型，防守型，全能型）

    XYQiziWidget  *curMovedQizi;        // 记录当前移动的棋子
    XYQiziWidget  *lastMovedQizi;       // 记录上一次移动的棋子
    QList<XYQiziWidget *> allMovedQizis;// 记录所有移动的棋子
    XYQipanStatus *curQipanStatus;      // 记录当前的棋盘状态
    XYQipanStatus *lastQipanStatus;     // 记录上次的棋盘状态
    QList<XYQipanStatus *> allStatus;   // 记录开局所走的所有的棋盘状态

    QPoint         targetPoint;         // 象棋巫师作为AI
};

#endif // XYAIQISHOU_H
