#include "vehicle.h"
#include "qdebug.h"
#include <math.h>

#define INVALID_VALUE -1

Vehicle::Vehicle(int street_id, double speed, direction dir, double progress)  :
    street_id(street_id), progress(progress), dir(dir), speed(speed)
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
