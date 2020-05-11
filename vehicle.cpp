#include "vehicle.h"
#include <QDebug>

Vehicle::Vehicle(Street_dir _street_dir, QString _line_name, double _progress, double _speed) :
    Vehicle(_street_dir, _line_name, NextColor(), _progress, _speed)
{
    // nothing
}

Vehicle::Vehicle(Street_dir _street_dir, QString _line_name, QPen _pen, double _progress, double _speed) :
     pen(_pen),
     street(_street_dir.first),
     direction(_street_dir.second),
     internal_street_index(0),
     progress(_progress),
     speed(_speed),
     vehicle_symbol(_line_name)
{
    // nothing
}

/*QString Vehicle::getSymbol() const
{
#define FIRST_SYMB 'A'
    static char currentSymbol = FIRST_SYMB;

    if (currentSymbol == 'Z' + 1)
        currentSymbol = FIRST_SYMB;

    return QString(currentSymbol++);
}*/

QString Vehicle::symbol() const
{
    return vehicle_symbol;
}

void Vehicle::invalidate()
{
    speed = SPEED_INVALID;
}

bool Vehicle::isinvalid()
{
    return speed == SPEED_INVALID;
}

double Vehicle::streetPercentage(double street_cost)
{
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
