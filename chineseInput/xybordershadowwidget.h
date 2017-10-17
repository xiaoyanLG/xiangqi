#ifndef XYBORDERSHADOWWIDGET_H
#define XYBORDERSHADOWWIDGET_H

#include <QWidget>

class XYBorderShadowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XYBorderShadowWidget(QWidget *parent = 0);
    ~XYBorderShadowWidget();

signals:
    void closed();

public slots:
    bool close();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    bool          mbLeftMousePressed;
    QPoint        moLastPos;

};

#endif // XYBORDERSHADOWWIDGET_H
