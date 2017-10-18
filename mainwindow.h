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
    void layoutQizi(bool keep = false);
    void switchViews();
    void showMessage(const QString &msg, int times = 3000);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    bool          mbLeftMousePressed;
    QPoint        moLastPos;

private:
    static MainWindow *instance;
    XYQishou *me;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
