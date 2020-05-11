#ifndef TRIP_H
#define TRIP_H

#include <QTime>

#include "street.h"
#include "vehicle.h"
#include "maptimer.h"
#include "util.h"

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
    std::vector<std::shared_ptr<Vehicle>> vehicles() const;

    void addStreetToRoute(Street s, Direction d = dir_forward);
    void addSpawn(QTime time);
    void setLastTime(QTime time);

    void updateVehiclesAt(QTime time);
    std::vector<std::shared_ptr<Vehicle>> createNewVehiclesAt(QTime time);

    std::vector<std::shared_ptr<Vehicle>> vehiclePool;
private:
    QPen pen;
    std::vector<QTime> departures; // when to spawn new vehicles
    const QString lineName; // "N95"
    std::vector<Street_dir> lineRoute;
    QTime *lastTime;
    std::vector<double> stopsPositions;

    void advanceVehicleRoute(std::shared_ptr<Vehicle> v);
    void updateVehiclePosition(std::shared_ptr<Vehicle> v, double elapsedMSecs);
    void initStopsPositions();
};

#endif // TRIP_H
