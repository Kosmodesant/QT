#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>
#include <QTime>


class StopWatch : public QObject
{
    Q_OBJECT

public:
    explicit StopWatch(QObject *parent = nullptr);
//    ~StopWatch();

signals:
    void sig_updateTime(const QString &time);
    void sig_updateCircleLog(const QString &log);
    void sig_clearCircleLog();

public slots:
    void startTimer();
    void stopTimer();
    void rcv_circleTime();
    void rcv_clearTime();
    void clearCircleLog();
    QString getCircleTime() const; // Геттер для strCircleTime

private slots:
    void updateTime();

private:
    QTimer *timer;
    QTime time;
    QTime stopTime;
    int circleTime;
    int count;
    QString strCurrentTime;
    QString strCircleTime;
};

#endif // STOPWATCH_H
