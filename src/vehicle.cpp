/* vehicle.cpp
 * Project: CPP
 * Description: Vehicle class - single trip departure
 * Authors: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 *          Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QDebug>

#include "trip.h"

#include "vehicle.h"

Vehicle::Vehicle(Street_dir *street_dir, const QString& line_name, double progress, double speed) :
     trip(nullptr),
     vehicle_symbol(line_name.at(line_name.size() - 1)),
     pen(NextColorPen()),
     street(&street_dir->first),
     direction(street_dir->second),
     internal_street_index(0),
     progress(progress),
     speed(speed)
{

}

Vehicle::Vehicle(Trip *trip, QTime spawn_time) : Vehicle(&trip->route().at(0),
                                        trip->name(),
                                        trip->StopsPositions().at(0))
{
    pen = NextColorPen(trip->Id());
    this->trip = trip;
    this->spawn_time = QTime(spawn_time);
}

QString Vehicle::symbol() const
{
    return vehicle_symbol;
}

void Vehicle::invalidate()
{
    speed = INVALIDSPEED;
}

bool Vehicle::isinvalid() const
{
    return speed == INVALIDSPEED;
}

double Vehicle::streetPercentage(double street_cost) const
{
    double tmp;
    if (direction == DIRFORWARD)
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

double Vehicle::streetPercentage(const Street* street) const
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

QPointF Vehicle::position() const
{
    return PositionOnLine(*street,streetPercentage(street));
}

QTime Vehicle::spawnTime() const
{
    return spawn_time;
}
