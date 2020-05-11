#include "maptimer.h"

#include <stdexcept>

MapTimer::MapTimer(int h, int m, int s, double multiplier, QObject *parent) : QObject(parent)
{
    setTime = new QTime(h, m, s);
    internalTimer = new QTimer(this);
    timeMultiplier = multiplier;

    this->setInterval(1000); // default value

    connect(internalTimer, &QTimer::timeout, this, &MapTimer::privateTimeout);
}

MapTimer::~MapTimer()
{
    // 'internalTimer' is destroyed automatically with MapTimer class
    // since it's its QObject parent

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
    setTime = new QTime(0, 0);
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
    updateTime(qRound(internalTimer->interval() * timeMultiplier), &setTime);
    emit this->timeout(*setTime);
}

void MapTimer::updateTime(int addMilliseconds, QTime **time)
{
    *time = new QTime( (*time)->addMSecs(addMilliseconds) );
    if (!(*time)->isValid())
        throw std::logic_error("invalid time");
}
