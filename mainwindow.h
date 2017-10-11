#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    bool          mbLeftMousePressed;
    QPoint        moLastPos;

private:
    QList<XYQiziWidget *> hong_qizis;
    QList<XYQiziWidget *> hei_qizis;
    static MainWindow *instance;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
