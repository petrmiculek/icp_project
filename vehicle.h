#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>
#include <QTime>
#include "street.h"
#include "util.h"

#define SPEED_INVALID -1

// using clauses are in street.h

#define dir_forward false
#define dir_backward true

class Vehicle
{
public:
    Vehicle(Street_dir _street_dir, QString _line_name, double _progress = 0.0, double _speed = speed_default);
    Vehicle(Street_dir _street_dir, QString _line_name, QPen pen, double _progress = 0.0, double _speed = speed_default);

    const QPen pen;

    Street street;
    Direction direction;
    unsigned int internal_street_index;
    double progress;
    double speed;
    double restMSecs; // milliseconds to rest (i.e. on a bus stop)

    QTime spawn_time;

    void invalidate();
    bool isinvalid();
    double streetPercentage(double street_cost);
    double streetPercentage(Street street);
    double fromMSecsToProgress(double msecs);
    double fromProgressToMSecs(double progress);
    QPointF position();

    QString symbol() const;
private:
    //QString getSymbol() const;
    QString vehicle_symbol;

    static constexpr double speed_default = 0.01;
};

#endif // VEHICLE_H
