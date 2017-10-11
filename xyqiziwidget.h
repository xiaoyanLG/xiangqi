#ifndef XYQIZIWIDGET_H
#define XYQIZIWIDGET_H

#include <QWidget>

class XYQiziWidget : public QWidget
{
    Q_OBJECT
public:
    enum TYPE{HONG_ZU, HONG_PAO, HONG_CHE, HONG_MA, HONG_XIANG, HONG_SI, HONG_JIANG,
              HEI_ZU, HEI_PAO, HEI_CHE, HEI_MA, HEI_XIANG, HEI_SI, HEI_JIANG};
    explicit XYQiziWidget(TYPE type, int times, QWidget *parent = 0);
    ~XYQiziWidget();
    QPoint getQiziPos();
    void setType(TYPE type, int times);
    TYPE getType() const;

    QPoint getPos() const;
    void setPos(const QPoint &pos);

    bool getBeEaten() const;
    void setBeEaten(bool beEaten);

public slots:
    void resizeQizi(const QSize &size);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPixmap getPixMapByType(TYPE type, bool force = false);

protected:
    bool          mbLeftMousePressed;
    QPoint        moLastPos;

private:
    TYPE type;
    int  times;
    QPoint pos;
    bool beEaten;
    QPixmap pixmap;

};

#endif // XYQIZIWIDGET_H
