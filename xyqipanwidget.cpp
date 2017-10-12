#include "xyqipanwidget.h"
#include <QPainter>
#include <QResizeEvent>
#include <qmath.h>
#include <QDebug>

static qreal lengthToPoint(const QPoint &p1, const QPoint &p2)
{
    qreal x = qAbs(p1.x() - p2.x());
    qreal y = qAbs(p1.y() - p2.y());
    return qSqrt((x*x) + (y*y));
}

XYQipanWidget *XYQipanWidget::instance = NULL;
XYQipanWidget *XYQipanWidget::getInstance()
{
    return instance;
}

XYQipanWidget::XYQipanWidget(QWidget *parent)
    : QWidget(parent), tempQizi(NULL)
{
    memset(qiziInqipan, 0, sizeof(XYQiziWidget *) * 9 * 10);
    qipanPixmap.load(":/xiangqi/qipan.png");

    instance = this;
}

XYQipanWidget::~XYQipanWidget()
{

}

void XYQipanWidget::clear()
{
    memset(qiziInqipan, 0, sizeof(XYQiziWidget *) * 9 * 10);
    historyQibus.clear();
}

void XYQipanWidget::putQiziToDefaultPos(XYQiziWidget *qizi, bool up)
{
    QPoint defaultPos = qizi->getQiziDefaultPos(up);
    int row = defaultPos.x();
    int column = defaultPos.y();
    qizi->move(allPos[row][column] -
            QPoint(qizi->width() / 2, qizi->height() / 2));

    if (qizi->type != XYQiziWidget::TEMP)
    {
        if (qiziInqipan[row][column] != NULL && qiziInqipan[row][column] != qizi)
        {
            qiziInqipan[row][column]->setBeEaten(true);
        }

        if (qizi->curPos.x() >= 0 && qizi->curPos.x() <= 9
                && qizi->curPos.y() >= 0 && qizi->curPos.y() <= 8)
        {
            qiziInqipan[qizi->curPos.x()][qizi->curPos.y()] = NULL;
        }
        qiziInqipan[row][column] = qizi;
    }

    qizi->curPos = QPoint(row, column);
}

void XYQipanWidget::putQizi(XYQiziWidget *qizi, int row, int column, bool addHistory)
{
    XYQiziWidget *eaten = NULL;
    QPoint lastPos = qizi->curPos;
    qizi->move(allPos[row][column] -
            QPoint(qizi->width() / 2, qizi->height() / 2));

    if (qizi->type != XYQiziWidget::TEMP)
    {
        if (qiziInqipan[row][column] != NULL && qiziInqipan[row][column] != qizi)
        {
            qiziInqipan[row][column]->setBeEaten(true);
            eaten = qiziInqipan[row][column];
        }

        if (qizi->curPos.x() >= 0 && qizi->curPos.x() <= 9
                && qizi->curPos.y() >= 0 && qizi->curPos.y() <= 8)
        {
            qiziInqipan[qizi->curPos.x()][qizi->curPos.y()] = NULL;
        }
        qiziInqipan[row][column] = qizi;
    }

    qizi->curPos = QPoint(row, column);

    if (addHistory)
    {
        XYQibu *curQibu = new XYQibu(qizi, lastPos, eaten);
        historyQibus.push(curQibu);
    }
}

void XYQipanWidget::moveToNearestPos(XYQiziWidget *qizi)
{
    QPoint nearestPos = getQiziCurNearestPos(qizi);
    int row = nearestPos.x();
    int column = nearestPos.y();
    if (tempQizi != NULL)
    {
        tempQizi->setVisible(false);
    }
    if (qizi->isMovable(row, column))
    {
        putQizi(qizi, row, column, true);
    }
    else
    {
        putQizi(qizi, qizi->curPos.x(), qizi->curPos.y(), false);
    }
}

XYQiziWidget *XYQipanWidget::getPositionQizi(int row, int column)
{
    return qiziInqipan[row][column];
}

void XYQipanWidget::setTempQizi(XYQiziWidget *qizi)
{
    tempQizi = qizi;
}

void XYQipanWidget::raiseTempQizi()
{
    if (tempQizi != NULL)
    {
        tempQizi->raise();
    }
}

void XYQipanWidget::showTempQizi(XYQiziWidget *qizi)
{
    QPoint nearestPos = getQiziCurNearestPos(qizi);
    int row = nearestPos.x();
    int column = nearestPos.y();

    if (tempQizi != NULL)
    {
        tempQizi->setVisible(true);
        tempQizi->pixmap = qizi->pixmap;

        if (qizi->isMovable(row, column))
        {
            putQizi(tempQizi, row, column, false);
        }
        else
        {
            putQizi(tempQizi, qizi->curPos.x(), qizi->curPos.y(), false);
        }
    }
}

void XYQipanWidget::revokeLastQibu()
{

    if (!historyQibus.isEmpty())
    {
        XYQibu *last = historyQibus.pop();
        if (last->target != NULL)
        {
            putQizi(last->target, last->curPos.x(), last->curPos.y(), false);
        }
        if (last->eatenQizi != NULL)
        {
            last->eatenQizi->setBeEaten(false);
            putQizi(last->eatenQizi, last->eatenQizi->curPos.x(), last->eatenQizi->curPos.y(), false);
        }
        delete last;
    }
}

void XYQipanWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing
                          | QPainter::TextAntialiasing
                          | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect(), qipanPixmap);

    // 画笔
    QPen pen = painter.pen();
    pen.setWidth(pen.width() + 1);
    painter.setPen(pen);

    // 中间参数
    qreal w = (width() - 100) / 8;
    qreal h = (height() - 100) / 9;
    int curX = 50;
    int curY = 50;
    qreal offsetX = 0;
    qreal offsetY = 0;

    // 画棋盘横线
    for (int i = 0; i < 10; ++i)
    {
        offsetY = h * i;
        painter.drawLine(QPointF(curX, curY + offsetY), QPointF(curX + w * 8, curY + offsetY));
    }
    // 画棋盘纵线
    for (int i = 0; i < 9; ++i)
    {
        offsetX = w * i;
        if (i == 0 || i == 8)
        {
            painter.drawLine(QPointF(curX + offsetX, curY), QPointF(curX + offsetX, curY + 9 * h));
        }
        else
        {
            painter.drawLine(QPointF(curX + offsetX, curY), QPointF(curX + offsetX, curY + 4 * h));
            painter.drawLine(QPointF(curX + offsetX, curY + 5 * h), QPointF(curX + offsetX, curY + 9 * h));
        }
    }

    // 画炮兵位置图标
    QPainterPath path = getPosPath(QPoint(curX, curY + h * 3), w, 1);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX, curY + h * 6), w, 1);
    painter.drawPath(path);

    path = getPosPath(QPoint(curX + w * 8, curY + h * 3), w, 2);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 8, curY + h * 6), w, 2);
    painter.drawPath(path);

    path = getPosPath(QPoint(curX + w, curY + h * 2), w, 3);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 2, curY + h * 3), w, 3);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 4, curY + h * 3), w, 3);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 6, curY + h * 3), w, 3);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 7, curY + h * 2), w, 3);
    painter.drawPath(path);

    path = getPosPath(QPoint(curX + w, curY + h * 7), w, 3);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 2, curY + h * 6), w, 3);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 4, curY + h * 6), w, 3);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 6, curY + h * 6), w, 3);
    painter.drawPath(path);
    path = getPosPath(QPoint(curX + w * 7, curY + h * 7), w, 3);
    painter.drawPath(path);

    // 画士移动路线
    painter.drawLine(QPointF(curX + w * 3, curY), QPointF(curX + w * 5, curY + h * 2));
    painter.drawLine(QPointF(curX + w * 3, curY + h * 2), QPointF(curX + w * 5, curY));

    painter.drawLine(QPointF(curX + w * 3, curY + h * 7), QPointF(curX + w * 5, curY + h * 9));
    painter.drawLine(QPointF(curX + w * 3, curY + h * 9), QPointF(curX + w * 5, curY + h * 7));

    // 画文字
    QFont font = painter.font();
    font.setPointSize(w * 0.55);
    font.setFamily(QString::fromStdWString(L"华文行楷"));
    painter.setFont(font);

    QRect cheheRect(curX, curY + 4 * h, w * 4, h);
    painter.drawText(cheheRect, QString::fromStdWString(L"楚河"), QTextOption(Qt::AlignCenter));

    painter.translate(curX + w * 8, curY + 5.0 * h);
    painter.rotate(180);
    QRect hanjieRect(0, 0, w * 4, h);
    painter.drawText(hanjieRect, QString::fromStdWString(L"汉界"), QTextOption(Qt::AlignCenter));
}

void XYQipanWidget::resizeEvent(QResizeEvent *event)
{
    qreal w = (event->size().width() - 100) / 8;
    qreal h = (event->size().height() - 100) / 9;
    int curX = 50;
    int curY = 50;
    // 获取棋盘位置
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            allPos[i][j] = QPoint(curX + j * w, curY + i * h);
        }
    }

    emit sizeChanged(event->size());
    QWidget::resizeEvent(event);
}

QPoint XYQipanWidget::getQiziCurNearestPos(XYQiziWidget *qizi)
{
    qreal w = qAbs(allPos[0][0].x() - allPos[0][1].x());
    qreal h = qAbs(allPos[0][0].y() - allPos[1][0].y());

    QPoint point = qizi->pos();
    point.setX(point.x() + qizi->width() / 2);
    point.setY(point.y() + qizi->height() / 2);

    int row = 0;
    int column = 0;
    if (point.y() < 50)
    {
        row = 0;
    }
    else
    {
        row = qRound((point.y() - 50) / h);
        if (row > 9)
        {
            row = 9;
        }
    }
    if (point.x() < 50)
    {
        column = 0;
    }
    else
    {
        column = qRound((point.x() - 50) / w);
        if (column > 8)
        {
            column = 8;
        }
    }

    return QPoint(row, column);
}

QPainterPath XYQipanWidget::getPosPath(const QPointF &point, qreal w, int type)
{
    QPainterPath path;
    qreal lineW = w/6;
    qreal spaceW = w/22;
    if (type & 1)
    {
        path.moveTo(point.x() + lineW, point.y() - spaceW);
        path.lineTo(point.x() + spaceW, point.y() - spaceW);
        path.lineTo(point.x() + spaceW, point.y() - lineW);

        path.moveTo(point.x() + lineW, point.y() + spaceW);
        path.lineTo(point.x() + spaceW, point.y() + spaceW);
        path.lineTo(point.x() + spaceW, point.y() + lineW);
    }
    if (type & 2)
    {
        path.moveTo(point.x() - lineW, point.y() - spaceW);
        path.lineTo(point.x() - spaceW, point.y() - spaceW);
        path.lineTo(point.x() - spaceW, point.y() - lineW);

        path.moveTo(point.x() - lineW, point.y() + spaceW);
        path.lineTo(point.x() - spaceW, point.y() + spaceW);
        path.lineTo(point.x() - spaceW, point.y() + lineW);
    }

    return path;
}

