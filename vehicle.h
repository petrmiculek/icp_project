#ifndef VEHICLE_H
#define VEHICLE_H

using direction = bool;

#define dir_forward false
#define dir_backward true

class Vehicle
{
public:
    Vehicle(int street_id, double speed, direction dir = dir_forward, double progress = 0);
    ~Vehicle();

    int street_id; // which street it's on
    int internal_street_index;
    double progress;
    direction dir;
    double speed;

    double streetPercentage(double street_cost);
    double fromMSecsToProgress(double msecs);
    double fromProgressToMSecs(double progress);
};

#endif // VEHICLE_H
