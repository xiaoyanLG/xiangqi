#ifndef XYBATTLEINFOWIDGET_H
#define XYBATTLEINFOWIDGET_H

#include <QWidget>
#include <QHostAddress>

class QListWidget;
class QListWidgetItem;
class QTextEdit;
class QLineEdit;
class QPushButton;
class XYBattleInfoWidget : public QWidget
{
    Q_OBJECT
public:
    static XYBattleInfoWidget *getInstance();
    explicit XYBattleInfoWidget(QWidget *parent = 0);
    ~XYBattleInfoWidget();
    QHostAddress getSendHostAddress();

signals:
    void sendMessage(const QHostAddress &address, const QString &msg);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void peopleUpline(const QString &name, const QHostAddress &address);
    void peopleOffline(const QString &name, const QHostAddress &address);
    void receiveData(const QString &from, const QString &data);

private slots:
    void qishouSideChanged();
    void exit();
    void zoomIn();
    void zoomOut();
    void layoutQizi();
    void revoked();
    void switchViews();
    void switchColor();
    void hosting();
    void receiveUDP();
    void sendUDP();
    void startAI();
    void sendMessage();
    void connectPeople(QListWidgetItem *item);

private:
    static XYBattleInfoWidget *instance;
    QMap<int, QHostAddress> allPeoplesMap;
    QMap<int, QPushButton *> funcBtns;
    QPixmap qipanPixmap;
    QListWidget *allOnlinePeoplesWidget;
    QTextEdit   *messageBox;
    QLineEdit   *sendMessageEdit;

};

#endif // XYBATTLEINFOWIDGET_H
