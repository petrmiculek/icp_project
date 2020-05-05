#include "maptimer.h"

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

void MapTimer::setInterval(int interval)
{
    internalTimer->setInterval(interval);
    emit intervalChanged();
}

int MapTimer::getInterval() const
{
    return internalTimer->interval();
}

void MapTimer::privateTimeout()
{
    qDebug("Map Timer caught internal timeout!");

    emit this->timeout();
}
