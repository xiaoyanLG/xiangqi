#ifndef XYAIQISHOU_H
#define XYAIQISHOU_H

#include <QObject>
#include "xyqipanwidget.h"

class XYAIQishou : public QObject
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
    void qiziMoved(XYQiziWidget *qizi, const QPoint &point);

private:
    static XYAIQishou     *instance;
    XYQipanWidget         *qipan;
    int                    level;
    XYQiziWidget::SIDETYPE sideType;
    AITYPE                 aiType;

    QList<QPoint>          hongLastMovablePoints;
    QList<QPoint>          hongCurMovablePoints;
    QList<QPoint>          heiLastMovablePoints;
    QList<QPoint>          heiCurMovablePoints;
};

#endif // XYAIQISHOU_H
