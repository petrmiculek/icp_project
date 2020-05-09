#include "vehicle.h"
#include "qdebug.h"
#include <math.h>

Vehicle::Vehicle(int street_id, double speed, QString symbol, direction dir, double progress)  :
    street_id(street_id), progress(progress), dir(dir), speed(speed), _symbol(symbol)
{
    internal_street_index = 0;
}

QString Vehicle::symbol() const
{
    return _symbol;
}

double Vehicle::streetPercentage(double street_cost)
{
    qDebug() << "Linka " << symbol() << ": " << dir;
    if (dir == dir_forward)
        return progress / street_cost * 100;
    else
        return 100 - (progress / street_cost * 100);
}

double Vehicle::fromMSecsToProgress(double msecs)
{
    return speed * msecs;
}

double Vehicle::fromProgressToMSecs(double progress)
{
    return progress / speed;
}
