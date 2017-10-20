#include "xyqipanwidget.h"
#include "xyqishou.h"
#include "xybattleinfowidget.h"
#include <QPainter>
#include <QResizeEvent>
#include <qmath.h>
#include <QMessageBox>
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
    : QWidget(parent),
      tempQizi(NULL),
      hong_jiang(NULL),
      lastSideType(XYQiziWidget::UNKNOWN),
      allMoveTimes(0)
{
    memset(qiziInqipan, 0, sizeof(XYQiziWidget *) * 9 * 10);
    qipanPixmap.load(":/xiangqi/qipan.png");

    initQizi();
    instance = this;
}

XYQipanWidget::~XYQipanWidget()
{

}

void XYQipanWidget::clear(bool clearHistory)
{
    memset(qiziInqipan, 0, sizeof(XYQiziWidget *) * 9 * 10);
    if (clearHistory)
    {
        qDeleteAll(historyQibus);
        historyQibus.clear();
        lastSideType = XYQiziWidget::UNKNOWN;
        allMoveTimes = 0;
    }
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

void XYQipanWidget::putQizi(XYQiziWidget *qizi, int row, int column, bool addHistory, bool animation)
{
    XYQiziWidget *eaten = NULL;
    QPoint lastPos = qizi->curPos;

    if (qizi->type != XYQiziWidget::TEMP)
    {
        if (qiziInqipan[row][column] != NULL && qiziInqipan[row][column] != qizi)
        {
            eaten = qiziInqipan[row][column];
        }

        if (qizi->curPos.x() >= 0 && qizi->curPos.x() <= 9
                && qizi->curPos.y() >= 0 && qizi->curPos.y() <= 8)
        {
            qiziInqipan[qizi->curPos.x()][qizi->curPos.y()] = NULL;
        }
        qiziInqipan[row][column] = qizi;
    }

    if (animation)
    {
        qizi->moveWithAnimation(allPos[row][column] -
                                QPoint(qizi->width() / 2, qizi->height() / 2), eaten);
    }
    else
    {
        qizi->move(allPos[row][column] -
                QPoint(qizi->width() / 2, qizi->height() / 2));
        if (eaten != NULL)
        {
            eaten->setBeEaten(true);
        }
    }

    qizi->curPos = QPoint(row, column);

    if (addHistory)
    {
        XYQibu *curQibu = new XYQibu(qizi, lastPos, eaten);
        historyQibus.push(curQibu);

        // 记录当前下子的棋方
        lastSideType = qizi->getSideType();

        // 当前移动棋子移动次数加1
        qizi->moveTimes++;
        allMoveTimes++;

        emit qiziMoved(qizi);
    }
}

void XYQipanWidget::revokeQizi(XYQiziWidget *qizi, int row, int column, bool beEaten)
{
    if (!beEaten)
    {
        // 记录当前下子的棋方
        if (qizi->getSideType() == XYQiziWidget::RED)
        {
            lastSideType = XYQiziWidget::BLACK;
        }
        else
        {
            lastSideType = XYQiziWidget::RED;
        }

        qizi->moveWithAnimation(allPos[row][column] -
                                QPoint(qizi->width() / 2, qizi->height() / 2));
    }
    else
    {
        qizi->move(allPos[row][column] -
                QPoint(qizi->width() / 2, qizi->height() / 2));
    }

    if (qizi->type != XYQiziWidget::TEMP)
    {
        if (qizi->curPos.x() >= 0 && qizi->curPos.x() <= 9
                && qizi->curPos.y() >= 0 && qizi->curPos.y() <= 8)
        {
            qiziInqipan[qizi->curPos.x()][qizi->curPos.y()] = NULL;
        }
        qiziInqipan[row][column] = qizi;
    }

    qizi->curPos = QPoint(row, column);
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

    if (lastSideType == qizi->getSideType())
    {
        emit showMessages(QString::fromStdWString(L"该对方下棋！"));

    }
    else if (XYQishou::getInstance()->getSideType() != qizi->getSideType())
    {
        emit showMessages(QString::fromStdWString(L"不能操作对方的棋子！"));
    }
    else if (qizi->isMovable(row, column))
    {
        XYQishou::getInstance()->sendQizi(
                    XYBattleInfoWidget::getInstance()->getSendHostAddress(),
                    qizi, QPoint(row, column), getQipanStatus(), false);
        putQizi(qizi, row, column, true);
        return;
    }
    else if (qizi->curPos != QPoint(row, column))
    {
        emit showMessages(QString::fromStdWString(L"该棋子不能走这里！"));
    }
    putQizi(qizi, qizi->curPos.x(), qizi->curPos.y(), false);
}

XYQiziWidget *XYQipanWidget::getPositionQizi(int row, int column)
{
    return qiziInqipan[row][column];
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

void XYQipanWidget::revokeLastQibu(bool socket)
{
    if (!historyQibus.isEmpty())
    {
        XYQibu *last = historyQibus.pop();
        if (last->target != NULL)
        {
            if (socket)
            {
                XYQishou::getInstance()->sendQizi(
                            XYBattleInfoWidget::getInstance()->getSendHostAddress(),
                            last->target, last->curPos, getQipanStatus(), true);
            }
            revokeQizi(last->target, last->curPos.x(), last->curPos.y(), false);

            // 棋子移动次数减1
            last->target->moveTimes--;
            allMoveTimes--;

            emit qiziMoved(last->target);
        }
        if (last->eatenQizi != NULL)
        {
            last->eatenQizi->setBeEaten(false);
            revokeQizi(last->eatenQizi, last->eatenPos.x(), last->eatenPos.y(), true);
        }
        delete last;
    }
}

XYQiziWidget *XYQipanWidget::findQizi(const QByteArray &key,
                                      XYQiziWidget::TYPE type,
                                      int times,
                                      const QPoint &lastPoint,
                                      QPoint &movePoint)
{
    XYQiziWidget *find = NULL;
    if (key != getQipanStatus())
    {
        return find;
    }
    for (int i = 0; i < hong_qizis.size(); ++i)
    {
        XYQiziWidget *qizi = hong_qizis.at(i);
        if (qizi->type == type && qizi->times == times)
        {
            if (lastPoint == qizi->curPos)
            {
                find = qizi;
            }
            else if (lastPoint == qizi->getSwitchViewsPos(qizi->curPos))
            {
                movePoint = qizi->getSwitchViewsPos(movePoint);
                find = qizi;
            }
            else
            {
                return NULL;
            }
        }
    }

    for (int i = 0; i < hei_qizis.size(); ++i)
    {
        XYQiziWidget *qizi = hei_qizis.at(i);
        if (qizi->type == type && qizi->times == times)
        {
            if (lastPoint == qizi->curPos)
            {
                find = qizi;
            }
            else if (lastPoint == qizi->getSwitchViewsPos(qizi->curPos))
            {
                movePoint = qizi->getSwitchViewsPos(movePoint);
                find = qizi;
            }
            else
            {
                return NULL;
            }
        }
    }

    return find;
}

QByteArray XYQipanWidget::getQipanStatus()
{
    QByteArray allPoints;
    for (int row = 0; row < 10; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            if (qiziInqipan[row][column] == NULL)
            {
                allPoints.append((char)0);
            }
            else
            {
                allPoints.append(qiziInqipan[row][column]->getIndex());
            }
        }
    }

    return allPoints;
}

XYQipanStatus *XYQipanWidget::getCurQipanStatus()
{
    XYQipanStatus *status = new XYQipanStatus(this);

    for (int row = 0; row < 10; ++row)
    {
        for (int column = 0; column < 9; ++column)
        {
            if (qiziInqipan[row][column] == NULL)
            {
                status->qipan[row][column] = XYQipanStatus::TYPE(0);
            }
            else
            {
                status->qipan[row][column] = XYQipanStatus::TYPE(qiziInqipan[row][column]->getIndex());
            }
        }
    }

    status->hongIsSmall = hong_jiang->defaultPos.x() < 5;

    return status;
}

void XYQipanWidget::layoutQizi(bool keep)
{
    static bool up = true;
    clear(true);
    if (keep)
    {
        up = !up;
    }
    for (int i = 0; i < hong_qizis.size(); ++i)
    {
        hong_qizis.at(i)->setVisible(true);
        hong_qizis.at(i)->moveTimes = 0;
        putQiziToDefaultPos(hong_qizis.at(i), up);
    }

    for (int i = 0; i < hei_qizis.size(); ++i)
    {
        hei_qizis.at(i)->setVisible(true);
        hei_qizis.at(i)->moveTimes = 0;
        putQiziToDefaultPos(hei_qizis.at(i), !up);
    }
    up = !up;
}

void XYQipanWidget::switchViews()
{
    clear(false);

    // 切换棋子位置
    for (int i = 0; i < hong_qizis.size(); ++i)
    {
        XYQiziWidget *qizi = hong_qizis.at(i);
        qizi->switchViews();
        if (!qizi->getBeEaten())
        {
            putQizi(qizi, qizi->getCurPos().x(), qizi->getCurPos().y(), false);
        }
    }

    for (int i = 0; i < hei_qizis.size(); ++i)
    {
        XYQiziWidget *qizi = hei_qizis.at(i);
        qizi->switchViews();
        if (!qizi->getBeEaten())
        {
            putQizi(qizi, qizi->getCurPos().x(), qizi->getCurPos().y(), false);
        }
    }

    // 切换历史记录的位置
    QStack<XYQibu *>::iterator it = historyQibus.begin();
    while (it != historyQibus.end())
    {
        (*it)->switchViews();
        ++it;
    }
}

void XYQipanWidget::moveQizi(XYQiziWidget *qizi, const QPoint &point, bool revoked)
{
    if (revoked)
    {
        revokeLastQibu(false);
    }
    else
    {
        putQizi(qizi, point.x(), point.y(), true, true);
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

void XYQipanWidget::initQizi()
{
    for (int i = XYQiziWidget::HONG_ZU; i <= XYQiziWidget::HONG_JIANG; ++i)
    {
        int times = 0;
        XYQiziWidget::TYPE type = (XYQiziWidget::TYPE)i;
        switch (type)
        {
        case XYQiziWidget::HONG_ZU:
            times = 5;
            break;
        case XYQiziWidget::HONG_JIANG:
            times = 1;
            break;
        default:
            times = 2;
            break;
        }

        for (int j = 0; j < times; ++j)
        {
            XYQiziWidget *qizi = new XYQiziWidget(type, j, this);
            if (type == XYQiziWidget::HONG_JIANG)
            {
                hong_jiang = qizi;
            }
            hong_qizis.append(qizi);
            connect(this, SIGNAL(sizeChanged(QSize)), qizi, SLOT(resizeQizi(QSize)));
        }
    }
    for (int i = XYQiziWidget::HEI_ZU; i <= XYQiziWidget::HEI_JIANG; ++i)
    {
        int times = 0;
        XYQiziWidget::TYPE type = (XYQiziWidget::TYPE)i;
        switch (type)
        {
        case XYQiziWidget::HEI_ZU:
            times = 5;
            break;
        case XYQiziWidget::HEI_JIANG:
            times = 1;
            break;
        default:
            times = 2;
            break;
        }

        for (int j = 0; j < times; ++j)
        {
            XYQiziWidget *qizi = new XYQiziWidget(type, j, this);
            hei_qizis.append(qizi);
            connect(this, SIGNAL(sizeChanged(QSize)), qizi, SLOT(resizeQizi(QSize)));
        }
    }

    // 准备临时棋子
    tempQizi = new XYQiziWidget(XYQiziWidget::TEMP, 0, this);
    connect(this, SIGNAL(sizeChanged(QSize)), tempQizi, SLOT(resizeQizi(QSize)));
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

