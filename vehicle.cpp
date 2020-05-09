#include "vehicle.h"

#include "qdebug.h"

Vehicle::Vehicle(int street_id, double speed, direction dir, double progress)  :
    street_id(street_id), progress(progress), dir(dir), speed(speed)
{
    internal_street_index = 0;
}

double Vehicle::streetPercentage(double street_cost)
{
    return (dir == dir_forward ? 1 : (-1)) * progress / street_cost * 100;
}

double Vehicle::fromMSecsToProgress(double msecs)
{
    return speed * msecs;
}

double Vehicle::fromProgressToMSecs(double progress)
{
    return progress / speed;
}
