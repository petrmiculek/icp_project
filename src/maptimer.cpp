/* maptimer.cpp
 * Project: CPP
 * Description: Timer sending periodic signals.
 * Author: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 */

#include "maptimer.h"

#include <stdexcept>

MapTimer::MapTimer(int h, int m, int s, double multiplier, QObject *parent) : QObject(parent)
{
    setTime = new QTime(h, m, s);
    internalTimer = new QTimer();
    timeMultiplier = multiplier;

    this->setInterval(1000); // default value

    connect(internalTimer, &QTimer::timeout, this, &MapTimer::privateTimeout);
}

MapTimer::~MapTimer()
{
    delete internalTimer;
    delete setTime;
}

QTime MapTimer::currentTime() const
{
    return QTime::currentTime();
}

QString MapTimer::currentTime(const QString &format) const
{
    return setTime->toString(format);
}

void MapTimer::start()
{
    if (!internalTimer->isActive()) {
        internalTimer->start();
    }
}

void MapTimer::stop()
{
    internalTimer->stop();
}

void MapTimer::reset()
{
    const bool startTimer = internalTimer->isActive();
    internalTimer->stop();
    setTime->setHMS(0, 0, 0);
    emit timeout(*setTime);
    emit reset_signal();
    if (startTimer)
        internalTimer->start();
}

bool MapTimer::isRunning() const
{
    return internalTimer->isActive();
}

void MapTimer::setInterval(int interval)
{
    internalTimer->setInterval(interval);
    emit intervalChanged(interval);
}

int MapTimer::getInterval() const
{
    return internalTimer->interval();
}

void MapTimer::setMultiplier(double multiplier)
{
    if (multiplier < 0.0)
        return;
    timeMultiplier = multiplier;
    emit multiplierChanged(multiplier);
}

double MapTimer::getMultiplier() const
{
    return timeMultiplier;
}

void MapTimer::privateTimeout()
{
    updateTime(qRound(internalTimer->interval() * timeMultiplier));
    emit this->timeout(*setTime);
}

void MapTimer::updateTime(int addMilliseconds)
{
    QTime tmp(setTime->addMSecs(addMilliseconds));
    setTime->setHMS(tmp.hour(), tmp.minute(), tmp.second(), tmp.msec());
}
