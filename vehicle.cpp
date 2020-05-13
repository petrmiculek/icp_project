#include "vehicle.h"
#include "trip.h"
#include <QDebug>



Vehicle::Vehicle(Street_dir _street_dir, QString _line_name, double _progress, double _speed) :
     pen(NextColorPen()),
     street(&_street_dir.first),
     direction(_street_dir.second),
     internal_street_index(0),
     progress(_progress),
     speed(_speed),
     vehicle_symbol(_line_name.back())
{

}

Vehicle::Vehicle(Trip *trip) :
    trip(trip)
{
    street = &trip->route().front().first;
    direction = trip->route().front().second;
    internal_street_index = 0;
    progress = trip->StopsPositions().front(); // first stop
    speed = speed_default;
    vehicle_symbol = trip->name().back();
    pen = NextColorPen(trip->Id());
}

/*
QString Vehicle::getSymbol() const
{
    #define FIRST_SYMB 'A'
    static char currentSymbol = FIRST_SYMB;

    if (currentSymbol == 'Z' + 1)
        currentSymbol = FIRST_SYMB;

    return QString(currentSymbol++);
}
*/

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

    if(tmp < 0.0)
    {
        return 0.0;
    }

    if(tmp > 100.0)
    {
        return 100.0;
    }

    return tmp;
}

double Vehicle::streetPercentage(const Street* street)
{
    return streetPercentage(street->time_cost);
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
    return PositionOnLine(*street,streetPercentage(street));
}
