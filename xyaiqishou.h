#ifndef XYAIQISHOU_H
#define XYAIQISHOU_H

#include <QThread>
#include "xyqipanwidget.h"

class XYAIQishou : public QThread
{
    Q_OBJECT
public:
    enum AITYPE{ATTACK, GUARD, AUTO};
    static XYAIQishou *getInstance();
    explicit XYAIQishou(QObject *parent = 0);
    ~XYAIQishou();
    void setQipan(XYQipanWidget *qipan);

signals:
    void moveQizi(XYQiziWidget *qizi, const QPoint &point);

public slots:
    void setLevel(int level);
    void setAiType(AITYPE type);
    void setSideType(XYQiziWidget::SIDETYPE type);
    void qiziMoved(XYQiziWidget *qizi);

private:
    static XYAIQishou     *instance;
    XYQipanWidget         *qipan;
    int                    level;       // 记录AI技术水平
    XYQiziWidget::SIDETYPE sideType;    // 记录AI走哪一方
    AITYPE                 aiType;      // 记录AI类型（进攻型，防守型，全能型）
    XYQiziWidget          *lastQizi;    // 记录上一次移动的棋子

    // qint64 存放棋子的指针，list存放该棋子能移动的所有坐标
    QMap<qint64, QList<QPoint> > hongLastMovablePoints;
    QMap<qint64, QList<QPoint> > hongCurMovablePoints;
    QMap<qint64, QList<QPoint> > heiLastMovablePoints;
    QMap<qint64, QList<QPoint> > heiCurMovablePoints;
};

#endif // XYAIQISHOU_H
