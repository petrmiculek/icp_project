#ifndef TRIP_H
#define TRIP_H

#include <QTime>

#include "street.h"
#include "vehicle.h"
#include "maptimer.h"

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

    void addStreetToRoute(Street s, Direction d = dir_forward);
    void addSpawn(QTime time);
    void setLastTime(QTime time);

    void updateVehiclesAt(QTime time);
    void createNewVehiclesAt(QTime time);

private:
    std::vector<Vehicle> vehiclePool;
    std::vector<Street_dir> lineRoute;
    std::vector<QTime> spawns; // when to spawn new vehicles
    const QString lineName; // "N95"
    QTime *lastTime;

    void advanceVehicleRoute(Vehicle* v);
    void updateVehiclePosition(Vehicle &v, double elapsedMSecs);
};

#endif // TRIP_H
