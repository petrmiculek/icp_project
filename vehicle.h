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

class Trip;

class Vehicle
{
public:
    Vehicle(Street_dir _street_dir, QString _line_name, double _progress = 0.0, double _speed = speed_default);
    Vehicle(Trip *trip);

    QPen pen;

    Street* street;
    Direction direction;
    unsigned int internal_street_index;
    double progress;
    double speed;
    double restMSecs; // milliseconds to rest (i.e. on a bus stop)

    QTime spawn_time;

    void invalidate();
    bool isinvalid();
    double streetPercentage(double street_cost);
    double streetPercentage(const Street* street);
    double fromMSecsToProgress(double msecs);
    double fromProgressToMSecs(double progress);
    QPointF position();
    Trip *trip;

    QString symbol() const;
private:
    //QString getSymbol() const;
    QString vehicle_symbol;

    static constexpr double speed_default =
#ifndef NDEBUG
    0.05; // debug speed
#else
    0.001; // release speed
#endif
};

#endif // VEHICLE_H
