#ifndef VEHICLE_H
#define VEHICLE_H

using direction = bool;

class Vehicle
{
public:
    Vehicle(int street_id, double speed, direction dir = true);
    ~Vehicle();
    int street_id;  // which street it's on
    int street_percentage;
    direction dir;
    double speed;
};

#endif // VEHICLE_H
