#ifndef XYQIZIWIDGET_H
#define XYQIZIWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>

class XYQiziWidget : public QWidget
{
    Q_OBJECT
public:
    enum SIDETYPE{RED, BLACK, UNKNOWN};
    enum TYPE{HONG_ZU, HONG_PAO, HONG_CHE, HONG_MA, HONG_XIANG, HONG_SI, HONG_JIANG,
              HEI_ZU, HEI_PAO, HEI_CHE, HEI_MA, HEI_XIANG, HEI_SI, HEI_JIANG, TEMP};
    explicit XYQiziWidget(TYPE type, int times, QWidget *parent = 0);
    ~XYQiziWidget();
    void moveWithAnimation(const QPoint &target, XYQiziWidget *finishedQizi = NULL);

    // 切换视角
    void switchViews();

    // 获取棋子默认的位置
    QPoint getQiziDefaultPos(bool up);

    // 判断位置棋子是否可以走
    bool isMovable(int row, int column);

    // 获取棋子是哪一方的
    SIDETYPE getSideType();

    void setType(TYPE type, int times);
    TYPE getType() const;

    bool getBeEaten() const;
    void setBeEaten(bool beEaten);

    QPoint getCurPos() const;
    static QPoint getSwitchViewsPos(const QPoint &point);
    void setCurPos(const QPoint &value);

    QPixmap getPixmap() const;
    void setPixmap(const QPixmap &value);

public slots:
    void resizeQizi(const QSize &size);
    void eatQizi();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPixmap getPixMapByType(TYPE type, bool force = false);

protected:
    QPropertyAnimation *mopMoveAnimation;
    bool          mbLeftMousePressed;
    QPoint        moLastPos;

private:
    XYQiziWidget *finishedQizi;
    TYPE    type;
    int     times;
    bool    beEaten;
    QPixmap pixmap;
    QPoint  curPos;        // 记录所在（行，列）
    QPoint  defaultPos;    // 记录初始位置

    friend class XYQipanWidget;
    friend class XYQishou;

};

#endif // XYQIZIWIDGET_H
