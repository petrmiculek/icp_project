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
    std::vector<Vehicle*> vehicles() const;

    void addStreetToRoute(Street s, direction d = true);
    void addSpawn(QTime time);
    void setLastTime(QTime time);

    void spawnVehiclesAt(QTime time);

private:
    std::vector<Vehicle*> vehiclePool;
    std::vector<std::tuple<Street, direction>> lineRoute;
    std::vector<QTime> spawns; // when to spawn new vehicles
    const QString lineName; // "N95"
    QTime *lastTime;

    void createNewVehiclesAt(QTime time);
    void updateVehiclesPosition(int elapsedSecs);
};

#endif // TRIP_H
