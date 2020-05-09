#ifndef TRIP_H
#define TRIP_H

#include <QTime>

#include "street.h"
#include "vehicle.h"
#include "maptimer.h"

using direction = bool;
using Street_dir = std::pair<Street, direction>;

#define dir_forward false
#define dir_backward true

class Trip
{
public:
    Trip(QString name);
    Trip(QString name, std::vector<Street_dir> route);
    ~Trip();

    QString name() const;
    std::vector<Street_dir> route() const;
    std::vector<Vehicle> vehicles() const;

    void addStreetToRoute(Street s, direction d = dir_forward);
    void addSpawn(QTime time);
    void setLastTime(QTime time);

    void spawnVehiclesAt(QTime time);

//private:
    std::vector<Vehicle> vehiclePool;
    std::vector<Street_dir> lineRoute;
    std::vector<QTime> spawns; // when to spawn new vehicles
    const QString lineName; // "N95"
    QTime *lastTime;

    void createNewVehiclesAt(QTime time);
    void advanceVehicleRoute(Vehicle* v);
    void updateVehiclePosition(Vehicle &v, double elapsedMSecs);
};

#endif // TRIP_H
