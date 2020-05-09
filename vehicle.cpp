#include "vehicle.h"

Vehicle::Vehicle(int street_id, double speed, direction dir)  :
    street_id(street_id), dir(dir), speed(speed)
{
    street_percentage = dir ? 0 : 100;
}
