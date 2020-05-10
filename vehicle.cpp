#include "vehicle.h"
#include "qdebug.h"
#include <math.h>

#define INVALID_VALUE -1

Vehicle::Vehicle(Street_dir _street_dir, double _speed, double _progress)  :
    street(_street_dir.first), direction(_street_dir.second), progress(_progress), speed(_speed)
{
    internal_street_index = 0;
    _symbol = getSymbol();
}

QString Vehicle::getSymbol() const
{
#define FIRST_SYMB 'A'
    static /*constinit*/ char currentSymbol = FIRST_SYMB;

    if (currentSymbol == 'Z' + 1)
        currentSymbol = FIRST_SYMB;

    return QString(currentSymbol++);
}

QString Vehicle::symbol() const
{
    return _symbol;
}

void Vehicle::invalidate()
{
    speed = INVALID_VALUE;
}

bool Vehicle::isinvalid()
{
    return speed == INVALID_VALUE;
}

double Vehicle::streetPercentage(double street_cost)
{
    if (street_cost == 0) {qDebug() << "div by zero attempted in streetPercentage"; return 0;}

    if (direction == dir_forward)
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
