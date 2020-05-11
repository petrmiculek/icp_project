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
    double tmp;
    if (direction == dir_forward)
        tmp = progress / street_cost * 100;
    else
        tmp = 100 - (progress / street_cost * 100);

    if(tmp < 0.0 || tmp > 100.0)
    {
        throw std::out_of_range("street_percentage");
    }
    return tmp;
}

double Vehicle::streetPercentage(Street street)
{
    return streetPercentage(street.time_cost);
}

double Vehicle::fromMSecsToProgress(double msecs)
{
    return speed * msecs;
}

double Vehicle::fromProgressToMSecs(double progress)
{
    return progress / speed;
}

QPointF Vehicle::position()
{
    return PositionOnLine(street,streetPercentage(street));
}
