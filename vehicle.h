#ifndef VEHICLE_H
#define VEHICLE_H

class Trip;

class Vehicle
{
public:
    Vehicle(int street_id, int street_percentage);
    int street_id;  // which street it's on
    int street_percentage;
};

#endif // VEHICLE_H
