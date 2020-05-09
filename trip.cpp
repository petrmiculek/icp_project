#include "trip.h"

#include <math.h>
#include <qdebug.h>

using namespace std;

Trip::Trip(QString name) : lineName(name), lastTime(nullptr)
{

}

Trip::Trip(QString name, vector<Street_dir> route) : Trip(name)
{
    this->lineRoute = route;
}

Trip::~Trip()
{
    delete lastTime;
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

void Trip::addStreetToRoute(Street s, direction d) // , direction d = dir_default (== true)
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

void Trip::advanceVehicleRoute(Vehicle *v)
{
    ++v->internal_street_index;
    if (v->internal_street_index < lineRoute.size()) {
        v->street_id = lineRoute[v->internal_street_index].first.id;
        v->dir = lineRoute[v->internal_street_index].second;
        v->progress = 0;
    }
    else {
        // invalidating vehicle
        v->speed = -1;
    }
}

void Trip::spawnVehiclesAt(QTime time)
{
    // updating existing vehicles
    if (lastTime && vehiclePool.size() != 0) {
        const int msecsElapsed = abs(time.msecsTo(*lastTime));
        for (auto& vehicle : vehiclePool) {
            updateVehiclePosition(vehicle, msecsElapsed);

            // we've reached the end of the street
            double w; // wasted progress
            while ((w=vehicle.progress - lineRoute[vehicle.internal_street_index].first.time_cost) > 0) {
                advanceVehicleRoute(&vehicle);

                w = vehicle.fromProgressToMSecs(w); // convert to wasted milliseconds

                updateVehiclePosition(vehicle, w); // advance again on new street
            }
        }
    }

    createNewVehiclesAt(time);

    setLastTime(time);
}

void Trip::updateVehiclePosition(Vehicle &v, double elapsedMSecs)
{
    if (elapsedMSecs == 0)
        return;

    v.progress += v.fromMSecsToProgress(elapsedMSecs);
}

void Trip::createNewVehiclesAt(QTime time)
{
    static char symb = 'A';
    if (spawns.size() == 0)
        return;

    auto& [street, direction] = lineRoute.front();

    if (!lastTime) {
        // first call, lastTime was not set yet
        for (auto t : spawns)
            if (t == time) {
                vehiclePool.push_back(Vehicle(street.id, 0.01, QString(symb++), direction));
            }
    }
    else {
        // lastTime set => check if time is past this point
        for (auto t : spawns)
            if (*lastTime < t && t <= time) {
                vehiclePool.push_back(Vehicle(street.id, 0.01, QString(symb++), direction));
            }
    }
}
