#include "stopwatch.h"
#include <QTimer>

StopWatch::StopWatch(QObject *parent) : QObject(parent)
{
    time.setHMS(0, 0, 0, 0);
    stopTime.setHMS(0, 0, 0, 0);
    circleTime = 0;
    count = 1;

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &StopWatch::updateTime);
}

//StopWatch::~StopWatch()
//{
//    //delete timer; // не нужно удалять таймер вручнуюб он  управляется объектной иерархией
//}

void StopWatch::updateTime()
{
    time = time.addMSecs(100);
    strCurrentTime = time.toString("mm:ss.zz");
    emit sig_updateTime(strCurrentTime);
}

void StopWatch::startTimer()
{
    timer->start();
    updateTime();
}

void StopWatch::stopTimer()
{
    timer->stop();
}

void StopWatch::rcv_circleTime()
{
    circleTime = stopTime.secsTo(time);
    strCircleTime = "Circle " + QString::number(count) + ", time: " + QString::number(circleTime) + " sec";
    stopTime = time;
    count++;
    emit sig_updateCircleLog(strCircleTime);
}


void StopWatch::rcv_clearTime()
{
    count = 1;
    time.setHMS(0, 0, 0, 0);
    stopTime.setHMS(0, 0, 0, 0);
    clearCircleLog();
}

void StopWatch::clearCircleLog()
{
    strCircleTime.clear();
    emit sig_clearCircleLog();
}

QString StopWatch::getCircleTime() const
{
    return strCircleTime;
}
