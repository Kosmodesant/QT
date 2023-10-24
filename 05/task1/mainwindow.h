#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sig_circleTime();
    void sig_clearTime();

private slots:
    void rcv_updateTime(const QString &text);
    void on_pB_start_clicked();
    void on_pB_circle_clicked();
    void on_pB_clear_clicked();

private:
    Ui::MainWindow *ui;
    StopWatch *stopwatch;
};

#endif // MAINWINDOW_H
