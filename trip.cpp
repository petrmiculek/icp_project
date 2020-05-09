#include "trip.h"

#include <math.h>
#include <qdebug.h>

using namespace std;

Trip::Trip(QString name) : lineName(name), lastTime(nullptr)
{
    // nothing, on purpose
}

Trip::Trip(QString name, vector<Street_dir> route) : Trip(name)
{
    this->lineRoute = route;
}

QString Trip::name() const
{
    return lineName;
}

vector<Street_dir> Trip::route() const
{
    return lineRoute;
}

vector<Vehicle> Trip::vehicles() const
{
    return vehiclePool;
}

void Trip::addStreetToRoute(Street s, direction d)
{
    lineRoute.emplace_back(s, d);
}

void Trip::addSpawn(QTime time)
{
    spawns.push_back(time);
}

void Trip::setLastTime(QTime time)
{
    lastTime = new QTime(time);
}

void Trip::spawnVehiclesAt(QTime time)
{
    if (lastTime)
        updateVehiclesPosition(abs(time.secsTo(*lastTime)));
    createNewVehiclesAt(time);

    setLastTime(time);
    // qDebug() << (*lastTime).toString();
}

void Trip::createNewVehiclesAt(QTime time)
{
    if (spawns.size() == 0)
        return;
    /*
    int street_id = get<0>(lineRoute.front()).id;
    bool direction = get<1>(lineRoute.front());
    */
    auto& [street, direction] = lineRoute.front();

    if (!lastTime) {
        for (auto t : spawns)
            if (t == time)
                vehiclePool.push_back(Vehicle(street.id, 0.1, direction));
    }
    else {
        for (auto t : spawns)
            if (*lastTime > t && t <= time)
                vehiclePool.push_back(Vehicle(street.id, 0.1, direction));
    }
}

void Trip::updateVehiclesPosition(int elapsedSecs)
{
    if (vehiclePool.size() == 0)
        return;
}
