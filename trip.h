#ifndef TRIP_H
#define TRIP_H

#include <QTime>

#include "street.h"
#include "vehicle.h"
#include "maptimer.h"

// using clauses are in street.h

#define dir_forward false
#define dir_backward true

class Trip
{
public:
    Trip(QString name);
    Trip(QString name, std::vector<Street_dir> route);
    Trip(QString name, std::vector<Street_dir> route, std::vector<QTime> departures);
    ~Trip();

    QString name() const;
    std::vector<Street_dir> route() const;
    std::vector<Vehicle> vehicles() const;

    void addStreetToRoute(Street s, Direction d = dir_forward);
    void addSpawn(QTime time);
    void setLastTime(QTime time);

    void updateVehiclesAt(QTime time);
    void createNewVehiclesAt(QTime time);

    // TODO Revert to private
    std::vector<QTime> departures; // when to spawn new vehicles
private:
    const QString lineName; // "N95"
    std::vector<Street_dir> lineRoute;
    QTime *lastTime;
    std::vector<Vehicle> vehiclePool;

    void advanceVehicleRoute(Vehicle* v);
    void updateVehiclePosition(Vehicle &v, double elapsedMSecs);
};

#endif // TRIP_H
