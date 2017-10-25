#include "xyqiziwidget.h"
#include "xyqipanwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>
#include <QDebug>

XYQiziWidget::XYQiziWidget(TYPE type, int times, QWidget *parent)
    : QWidget(parent),
      beEaten(false),
      curPos(QPoint(-1, -1)),
      finishedQizi(NULL),
      moveTimes(0)
{
    mopMoveAnimation = new QPropertyAnimation(this, "pos");
    mopMoveAnimation->setDuration(500);
    connect(mopMoveAnimation, SIGNAL(finished()), this, SLOT(eatQizi()));
    setType(type, times);
    setVisible(false);
}

XYQiziWidget::~XYQiziWidget()
{

}

void XYQiziWidget::moveWithAnimation(const QPoint &target, XYQiziWidget *finishedQizi)
{
    this->finishedQizi = finishedQizi;

    mopMoveAnimation->setStartValue(pos());
    mopMoveAnimation->setEndValue(target);
    mopMoveAnimation->start();
    raise();
}

void XYQiziWidget::switchViews()
{
    defaultPos = getSwitchViewsPos(defaultPos);

    if (!beEaten)
    {
        curPos = getSwitchViewsPos(curPos);
    }
}

QPoint XYQiziWidget::getQiziDefaultPos(bool up)
{
    int row = 0;
    int column = 0;
    switch (type)
    {
    case HONG_ZU:
    case HEI_ZU:
        row = up ? 3 : 6;
        column = 0 + 2 * times;
        break;
    case HONG_PAO:
    case HEI_PAO:
        row = up ? 2 : 7;
        column = 1 + 6 * times;
        break;
    case HONG_CHE:
    case HEI_CHE:
        row = up ? 0 : 9;
        column = 0 + 8 * times;
        break;
    case HONG_MA:
    case HEI_MA:
        row = up ? 0 : 9;
        column = 1 + 6 * times;
        break;
    case HONG_XIANG:
    case HEI_XIANG:
        row = up ? 0 : 9;
        column = 2 + 4 * times;
        break;
    case HONG_SI:
    case HEI_SI:
        row = up ? 0 : 9;
        column = 3 + 2 * times;
        break;
    case HONG_JIANG:
    case HEI_JIANG:
        row = up ? 0 : 9;
        column = 4;
        break;
    case TEMP:
        row = -1;
        column = -1;
        break;
    default:
        break;
    }

    // 是否需要颠倒顺序
    if (up)
    {
        column = 8 - column;
    }
    defaultPos = QPoint(row, column);
    curPos = QPoint(-1, -1);
    return defaultPos;
}

bool XYQiziWidget::isMovable(int row, int column)
{
    XYQipanWidget *qipan = qobject_cast<XYQipanWidget *>(parentWidget());
    bool yes = false;
    switch (type)
    {
    case HONG_ZU:
    case HEI_ZU:
        if (qAbs(row - curPos.x()) + qAbs(column - curPos.y()) == 1
                && (defaultPos.x() <= 4 && row >= curPos.x()
                    || defaultPos.x() >= 5 && row <= curPos.x()) )
        {
            // 没有过河不能左右移动
            if ((defaultPos.x() <= 4 && row <= 4 && defaultPos.y() == column)
                    || (defaultPos.x() >= 5 && row >= 5 && defaultPos.y() == column)
                    || (defaultPos.x() <= 4 && row >= 5)
                    || (defaultPos.x() >= 5 && row <= 4) )
            {
                yes = true;
            }
        }
        break;
    case HONG_PAO:
    case HEI_PAO:
        if (row == curPos.x())
        {
            int fuhao = column > curPos.y() ? 1 : -1;
            int count = qAbs(column - curPos.y());
            int obstacleTimes = 0;
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = qipan->getPositionQizi(curPos.x(),
                                                                    curPos.y() + fuhao * i);
                if (obstacleQizi != NULL)
                {
                    obstacleTimes++;
                }
            }
            XYQiziWidget *lastQizi = qipan->getPositionQizi(row, column);
            if ((obstacleTimes == 1 && lastQizi != NULL)
                    || (obstacleTimes == 0 && lastQizi == NULL))
            {
                yes = true;
            }
        }
        else if (column == curPos.y())
        {
            int fuhao = row > curPos.x() ? 1 : -1;
            int count = qAbs(row - curPos.x());
            int obstacleTimes = 0;
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = qipan->getPositionQizi(curPos.x() + fuhao * i,
                                                                    curPos.y());
                if (obstacleQizi != NULL)
                {
                    obstacleTimes++;
                }
            }
            XYQiziWidget *lastQizi = qipan->getPositionQizi(row, column);
            if ((obstacleTimes == 1 && lastQizi != NULL)
                    || (obstacleTimes == 0 && lastQizi == NULL))
            {
                yes = true;
            }
        }
        break;
    case HONG_CHE:
    case HEI_CHE:
        if (row == curPos.x())
        {
            int fuhao = column > curPos.y() ? 1 : -1;
            int count = qAbs(column - curPos.y());
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = qipan->getPositionQizi(curPos.x(),
                                                                    curPos.y() + fuhao * i);
                if (obstacleQizi != NULL)
                {
                    return false;
                }
            }
            yes = true;
        }
        else if (column == curPos.y())
        {
            int fuhao = row > curPos.x() ? 1 : -1;
            int count = qAbs(row - curPos.x());
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = qipan->getPositionQizi(curPos.x() + fuhao * i,
                                                                    curPos.y());
                if (obstacleQizi != NULL)
                {
                    return false;
                }
            }
            yes = true;
        }
        break;
    case HONG_MA:
    case HEI_MA:
        if (qAbs(row - curPos.x()) + qAbs(column - curPos.y()) == 3)
        {
            // 判断是否有挡路棋子
            if (qAbs(row - curPos.x()) == 2)
            {
                int fuhao = row > curPos.x() ? 1 : -1;
                XYQiziWidget *obstacleQizi = qipan->getPositionQizi(curPos.x() + fuhao,
                                                                    curPos.y());
                if (obstacleQizi != NULL)
                {
                    return false;
                }

            }
            else if (qAbs(column - curPos.y()) == 2)
            {
                int fuhao = column > curPos.y() ? 1 : -1;
                XYQiziWidget *obstacleQizi = qipan->getPositionQizi(curPos.x(),
                                                                    curPos.y() + fuhao);
                if (obstacleQizi != NULL)
                {
                    return false;
                }
            }
            yes = true;
        }
        break;
    case HONG_XIANG:
    case HEI_XIANG:
        if (qAbs(row - curPos.x()) == 2
                && qAbs(column - curPos.y()) == 2
                && (row <= 4 && curPos.x() <= 4
                    || row >= 5 && curPos.x() >= 5) )
        {
            // 判断是否有挡路棋子
            XYQiziWidget *obstacleQizi = qipan->getPositionQizi((curPos.x() + row)/2,
                                                                (curPos.y() + column)/2);
            if (obstacleQizi != NULL)
            {
                return false;
            }
            yes = true;
        }
        break;
    case HONG_SI:
    case HEI_SI:
        if (qAbs(row - curPos.x()) == 1
                && qAbs(column - curPos.y()) == 1
                && (column >= 3 && column <= 5)
                && (row >= 7 || row <= 2) )
        {
            yes = true;
        }
        break;
    case HONG_JIANG:
    case HEI_JIANG:
        if (qAbs(row - curPos.x()) + qAbs(column - curPos.y()) == 1
                && (column >= 3 && column <= 5)
                && ((defaultPos.x() > 5 && row >= 7)
                || (defaultPos.x() < 5 && row <= 2)))
        {
            yes = true;
        }
        else if (curPos.y() == column)
        {
            int fuhao = row > curPos.x() ? 1 : -1;
            int count = qAbs(row - curPos.x());
            for (int i = 1; i < count; ++i)
            {
                XYQiziWidget *obstacleQizi = qipan->getPositionQizi(curPos.x() + fuhao * i, curPos.y());
                if (obstacleQizi != NULL)
                {
                    return false;
                }
            }
            XYQiziWidget *obstacleQizi = qipan->getPositionQizi(row, column);
            if (obstacleQizi != NULL)
            {
                if (obstacleQizi->type == HEI_JIANG
                        || obstacleQizi->type == HONG_JIANG)
                {
                    yes = true;
                }
            }
        }
        break;
    default:
        break;
    }

    // 同一方的棋子不能覆盖
    XYQiziWidget *lastQizi = qipan->getPositionQizi(row, column);

    if (lastQizi != NULL && (lastQizi->type <= HONG_JIANG && type <= HONG_JIANG
            || lastQizi->type > HONG_JIANG && type > HONG_JIANG))
    {
        yes = false;
    }
    return yes;
}

XYQiziWidget::SIDETYPE XYQiziWidget::getSideType()
{
    if (type >= HONG_ZU
            && type <= HONG_JIANG)
    {
        return RED;
    }
    else if (type >= HEI_ZU
             && type <= HEI_JIANG)
    {
        return BLACK;
    }

    return UNKNOWN;
}

int XYQiziWidget::getIndex()
{
    int i = 0;
    int ret = 0;
    while (i < type)
    {
        switch (i)
        {
        case HONG_ZU:
        case HEI_ZU:
            ret += 5;
            break;
        case HONG_JIANG:
        case HEI_JIANG:
            ret += 1;
            break;
        default:
            ret += 2;
            break;
        }
        ++i;
    }

    ret += times + 1;
    return ret;
}

void XYQiziWidget::setType(XYQiziWidget::TYPE type, int times)
{
    this->type = type;
    this->times = times;
    getPixMapByType(type, true);
}

XYQiziWidget::TYPE XYQiziWidget::getType() const
{
    return this->type;
}

void XYQiziWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    if (type == TEMP)
    {
        painter.setOpacity(0.55);
    }
    painter.drawPixmap(rect(), getPixMapByType(type));
//    painter.drawText(rect(), QString::number(times));
//    painter.drawText(rect(), QString::number(moveTimes));
//    painter.drawText(rect(), QString::number(getIndex()));
}

void XYQiziWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = true;
        moLastPos = event->globalPos();
    }
    XYQipanWidget *qipan = qobject_cast<XYQipanWidget *>(parentWidget());
    qipan->raiseTempQizi();
    raise();
}

void XYQiziWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = false;
        moLastPos = event->globalPos();
        XYQipanWidget *qipan = qobject_cast<XYQipanWidget *>(parentWidget());
        qipan->moveToNearestPos(this);
    }
}

void XYQiziWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mbLeftMousePressed)
    {
        QPoint lastpos(x(), y());
        lastpos.setX( lastpos.x() + event->globalX() - moLastPos.x());
        lastpos.setY( lastpos.y() + event->globalY() - moLastPos.y());
        move(lastpos);
        moLastPos = event->globalPos();
        XYQipanWidget *qipan = qobject_cast<XYQipanWidget *>(parentWidget());
        qipan->showTempQizi(this);
    }
}

QPixmap XYQiziWidget::getPixMapByType(XYQiziWidget::TYPE type, bool force)
{
    if (force || this->pixmap.isNull())
    {
        QPixmap pixmap;
        switch (type)
        {
        case HONG_ZU:
            pixmap.load(":/xiangqi/hong_zu.png");
            break;
        case HONG_PAO:
            pixmap.load(":/xiangqi/hong_pao.png");
            break;
        case HONG_CHE:
            pixmap.load(":/xiangqi/hong_che.png");
            break;
        case HONG_MA:
            pixmap.load(":/xiangqi/hong_ma.png");
            break;
        case HONG_XIANG:
            pixmap.load(":/xiangqi/hong_xiang.png");
            break;
        case HONG_SI:
            pixmap.load(":/xiangqi/hong_si.png");
            break;
        case HONG_JIANG:
            pixmap.load(":/xiangqi/hong_jiang.png");
            break;
        case HEI_ZU:
            pixmap.load(":/xiangqi/hei_zu.png");
            break;
        case HEI_PAO:
            pixmap.load(":/xiangqi/hei_pao.png");
            break;
        case HEI_CHE:
            pixmap.load(":/xiangqi/hei_che.png");
            break;
        case HEI_MA:
            pixmap.load(":/xiangqi/hei_ma.png");
            break;
        case HEI_XIANG:
            pixmap.load(":/xiangqi/hei_xiang.png");
            break;
        case HEI_SI:
            pixmap.load(":/xiangqi/hei_si.png");
            break;
        case HEI_JIANG:
            pixmap.load(":/xiangqi/hei_jiang.png");
            break;
        case TEMP:
            pixmap.load(":/xiangqi/hei_jiang.png");
            break;
        default:
            break;
        }
        this->pixmap = pixmap;
    }

    return this->pixmap;
}

QPixmap XYQiziWidget::getPixmap() const
{
    return pixmap;
}

void XYQiziWidget::setPixmap(const QPixmap &value)
{
    pixmap = value;
}

QPoint XYQiziWidget::getCurPos() const
{
    return curPos;
}

QPoint XYQiziWidget::getSwitchViewsPos(const QPoint &point)
{
    int x = 9 - point.x();
    int y = 8 - point.y();
    return QPoint(x, y);
}

void XYQiziWidget::setCurPos(const QPoint &value)
{
    curPos = value;
}

bool XYQiziWidget::getBeEaten() const
{
    return beEaten;
}

void XYQiziWidget::setBeEaten(bool beEaten)
{
    if (beEaten)
    {
        setVisible(false);
    }
    else
    {
        setVisible(true);
    }

    this->beEaten = beEaten;
}

void XYQiziWidget::resizeQizi(const QSize &size)
{
    static QPixmap qipan_pixmap(":/xiangqi/qipan.png");
    qreal ratio = 0.9 * size.height() / qipan_pixmap.size().height();
    QPixmap qizi = getPixMapByType(type);
    resize(qizi.size() * ratio);

    XYQipanWidget *qipan = qobject_cast<XYQipanWidget *>(parentWidget());
    qipan->putQizi(this, curPos.x(), curPos.y(), false, false, false);
}

void XYQiziWidget::eatQizi()
{
    if (finishedQizi != NULL)
    {
        finishedQizi->setBeEaten(true);
        finishedQizi->curPos = QPoint(-1, -1);
        finishedQizi = NULL;
    }
}
