#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "xyqishou.h"

namespace Ui {
class MainWindow;
}
class XYQiziWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow *getInstance();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void testsssss();

public slots:
    void layoutQizi();
    void switchViews();
    void appendMessage(const QString &from, const QString &message);
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showMessage(const QString &msg);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    bool          mbLeftMousePressed;
    QPoint        moLastPos;

private:
    XYQishou *me;
    QList<XYQiziWidget *> hong_qizis;
    QList<XYQiziWidget *> hei_qizis;
    XYQiziWidget *tempQizi;
    static MainWindow *instance;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
