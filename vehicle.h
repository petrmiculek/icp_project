#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>

using direction = bool;

#define dir_forward false
#define dir_backward true

class Vehicle
{
public:
    Vehicle(int street_id, double speed, QString symbol = "", direction dir = dir_forward, double progress = 0);

    int street_id; // which street it's on
    int internal_street_index;
    double progress;
    direction dir;
    double speed;

    double streetPercentage(double street_cost);
    double fromMSecsToProgress(double msecs);
    double fromProgressToMSecs(double progress);

    QString symbol() const;
private:
    QString _symbol;
};

#endif // VEHICLE_H
