#include "xyqiziwidget.h"
#include "xyqipanwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

XYQiziWidget::XYQiziWidget(TYPE type, int times, QWidget *parent)
    : QWidget(parent)
{
    setType(type, times);
    setVisible(false);
}

XYQiziWidget::~XYQiziWidget()
{

}

QPoint XYQiziWidget::getQiziPos(bool up)
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
    default:
        break;
    }
    return QPoint(row, column);
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
    painter.drawPixmap(rect(), getPixMapByType(type));
}

void XYQiziWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = true;
        moLastPos = event->globalPos();
    }
    raise();
}

void XYQiziWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = false;
        moLastPos = event->globalPos();
        XYQipanWidget::getInstance()->moveToNearestPos(this);
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
        default:
            break;
        }
        this->pixmap = pixmap;
    }

    return this->pixmap;
}
QPoint XYQiziWidget::getCurPos() const
{
    return curPos;
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
    this->beEaten = beEaten;
}

void XYQiziWidget::resizeQizi(const QSize &size)
{
    static QPixmap qipan(":/xiangqi/qipan.png");
    qreal ratio = 0.9 * size.width() / qipan.size().width();
    QPixmap qizi = getPixMapByType(type);
    resize(qizi.size() * ratio);

    XYQipanWidget::getInstance()->putQizi(this, curPos.x(), curPos.y());
}
