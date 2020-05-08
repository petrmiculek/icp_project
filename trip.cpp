#include "trip.h"

using namespace std;

Trip::Trip(QString name)
{
    lineName = name;
}
Trip::Trip(QString name, vector<tuple<Street, direction>> route) : Trip(name)
{
    this->lineRoute = route;
}

QString Trip::name() const
{
    return lineName;
}

vector<tuple<Street, direction>> Trip::route() const
{
    return lineRoute;
}
/*
void Trip::addStreetToRoute(Street s, direction d)
{
    lineRoute.push_back(tuple(s, d));
    emit routeChanged();
}
*/

void Trip::addSpawn(QTime time, direction d)
{
    spawns.push_back(tuple(time, d));
}

void Trip::spawnVehiclesAt(QTime atTime)
{
    for (auto t : spawns)
        if (get<0>(t) == atTime);
            //vehiclePool.push_back(Vehicle(0, 0));
}
