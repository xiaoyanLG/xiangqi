#ifndef XYBATTLEINFOWIDGET_H
#define XYBATTLEINFOWIDGET_H

#include <QWidget>
#include <QHostAddress>

class QListWidget;
class QListWidgetItem;
class QTextEdit;
class QLineEdit;
class XYBattleInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XYBattleInfoWidget(QWidget *parent = 0);
    ~XYBattleInfoWidget();

signals:
    void sendMessage(const QHostAddress &address, const QString &msg);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void peopleUpline(const QString &name, const QHostAddress &address);
    void peopleOffline(const QString &name, const QHostAddress &address);
    void receiveData(const QString &from, const QString &data);

private slots:
    void exit();
    void zoomIn();
    void zoomOut();
    void layoutQizi();
    void revoked();
    void sendMessage();

private:
    QMap<int, QHostAddress> allPeoplesMap;
    QPixmap qipanPixmap;
    QListWidget *allOnlinePeoplesWidget;
    QTextEdit   *messageBox;
    QLineEdit   *sendMessageEdit;

};

#endif // XYBATTLEINFOWIDGET_H
