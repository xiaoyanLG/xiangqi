#ifndef XYTHINKINGBOX_H
#define XYTHINKINGBOX_H

#include <QWidget>

class QLabel;
class XYThinkingBox : public QWidget
{
    Q_OBJECT
public:
    explicit XYThinkingBox(const QString &text, QWidget *parent = 0);
    ~XYThinkingBox();

public slots:
    void start();
    void stop();

protected:
    void timerEvent(QTimerEvent *event);

private:
    int timeID;
    int times;
    QMovie *movie;
    QLabel *timeLabel;
};

#endif // XYTHINKINGBOX_H
