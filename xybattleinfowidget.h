#ifndef XYBATTLEINFOWIDGET_H
#define XYBATTLEINFOWIDGET_H

#include <QWidget>

class XYBattleInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XYBattleInfoWidget(QWidget *parent = 0);
    ~XYBattleInfoWidget();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void exit();
    void zoomIn();
    void zoomOut();
    void layoutQizi();
    void revoked();

private:
    QPixmap qipanPixmap;

};

#endif // XYBATTLEINFOWIDGET_H
