#ifndef VEHICLE_H
#define VEHICLE_H

#include "trip.h"

class Vehicle
{
    int id;
    int street_id;  // which street it's on
    int street_percentage;
    Trip * trip;

};

#endif // VEHICLE_H
