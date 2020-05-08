#ifndef TRIP_H
#define TRIP_H

#include <QTime>

#include "street.h"
#include "vehicle.h"
#include "maptimer.h"

using direction = bool;

class Trip
{
public:
    Trip(QString name);
    Trip(QString name, std::vector<std::tuple<Street, direction>> route);

    QString name() const;
    std::vector<std::tuple<Street, direction>> route() const;

    void addStreetToRoute(Street s, direction d = true);
    void addSpawn(QTime time);

    void spawnVehiclesAt(QTime atTime);

private:
    QString lineName; // "N95"
    std::vector<std::tuple<Street, direction>> lineRoute;
    std::vector<std::tuple<QTime>> spawns; // when/how to spawn new vehicles
    std::vector<Vehicle> vehiclePool;
};

#endif // TRIP_H
