#ifndef XYQIPANWIDGET_H
#define XYQIPANWIDGET_H

#include <QWidget>
#include "xyqiziwidget.h"

class XYQipanWidget : public QWidget
{
    Q_OBJECT
public:
    static XYQipanWidget *getInstance();
    explicit XYQipanWidget(QWidget *parent = 0);
    ~XYQipanWidget();
    void putQizi(XYQiziWidget *qizi, int row, int column);
    void moveToNearestPos(XYQiziWidget *qizi);

signals:
    void sizeChanged(const QSize &size);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QPainterPath getPosPath(const QPointF &point, qreal w, int type); // type: 1, 2, 1|2

private:
    static XYQipanWidget *instance;
    QPixmap qipanPixmap;
    QPoint  allPos[10][9];
    bool    alreadyOccupy[10][9];
    XYQiziWidget *qiziInqipan[10][9];

};

#endif // XYQIPANWIDGET_H
