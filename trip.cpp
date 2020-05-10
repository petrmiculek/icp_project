#include "trip.h"

#include <math.h>
#include <qdebug.h>

using namespace std;

Trip::Trip(QString name) : lineName(name), lastTime (nullptr)
{

}

Trip::Trip(QString name, vector<Street_dir> route) : lineName(name), lastTime (nullptr)
{
    this->lineRoute = route;
}

Trip::Trip(QString name, std::vector<Street_dir> route, std::vector<QTime> _departures) :
    lineName(name),
    // lineRoute({route}), // TODO TEST IF THIS WOULD WORK
    // departures(std::move(_departures)),
    lastTime(nullptr)
{
    this->lineRoute = route;
    this->departures = _departures;
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

void Trip::addStreetToRoute(Street s, Direction d) // default param: Direction d = dir_forward
{
    lineRoute.emplace_back(s, d);
}

void Trip::addSpawn(QTime time)
{
    departures.push_back(time);
}

void Trip::setLastTime(QTime time)
{
    lastTime = new QTime(time);
}

void Trip::advanceVehicleRoute(Vehicle *v)
{
    v->internal_street_index++;
    if (v->internal_street_index < lineRoute.size()) {
        std::tie(v->street, v->direction) = lineRoute.at(v->internal_street_index);
        v->progress = 0;
    }
    else {
        // invalidating vehicle
        v->speed = SPEED_INVALID;
    }
}

void Trip::updateVehiclesAt(QTime time)
{
    // updating existing vehicles
    if (lastTime && vehiclePool.size() != 0) {
        const int msecsElapsed = abs(time.msecsTo(*lastTime));
        for (auto& vehicle : vehiclePool) {
            updateVehiclePosition(vehicle, msecsElapsed);

            // we've reached the end of the street
            double w; // wasted progress
            while (vehicle.internal_street_index < lineRoute.size() &&
                   (w=vehicle.progress - lineRoute.at(vehicle.internal_street_index).first.time_cost) > 0) {
                advanceVehicleRoute(&vehicle);

                w = vehicle.fromProgressToMSecs(w); // convert to wasted milliseconds

                updateVehiclePosition(vehicle, w); // advance again on new street
            }
        }
    }
}

void Trip::updateVehiclePosition(Vehicle &v, double elapsedMSecs)
{
    if (elapsedMSecs == 0)
        return;

    v.progress += v.fromMSecsToProgress(elapsedMSecs);
}

void Trip::createNewVehiclesAt(QTime time)
{
    if (departures.size() == 0)
        return;

    // unpacking not needed currently, keeping for reference
    // auto& [street, Direction] = lineRoute.front();

    if (!lastTime) {
        // first call, lastTime was not set yet
        for (auto t : departures)
            if (t == time) {
                vehiclePool.push_back(Vehicle(lineRoute.front(), name()));
            }
    }
    else {
        // lastTime set => check if time is past this point
        for (auto t : departures)
            if (*lastTime < t && t <= time) {
                vehiclePool.push_back(Vehicle(lineRoute.front(), name()));
            }
    }
}
