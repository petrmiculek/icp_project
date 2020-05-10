#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>
#include "street.h"

// using clauses are in street.h

#define dir_forward false
#define dir_backward true

class Vehicle
{
public:
    Vehicle(Street_dir& _street_dir, double _speed = speed_default, double _progress = 0.0);

    Street& street;
    Direction direction;
    unsigned int internal_street_index;
    double progress;
    double speed;

    void invalidate();
    bool isinvalid();
    double streetPercentage(double street_cost);
    double fromMSecsToProgress(double msecs);
    double fromProgressToMSecs(double progress);

    QString symbol() const;
private:
    QString getSymbol() const;
    QString _symbol;

    static constexpr double speed_default = 0.01;
};

#endif // VEHICLE_H
