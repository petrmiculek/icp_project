/* trip.cpp
 * Project: CPP
 * Description: Trip class represents a (bus) line - its route and departures
 * Authors: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 *          Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <cmath>
#include <QDebug>
#include <utility> 
#include "maptimer.h"
#include "util.h"

#include "trip.h"

using namespace std;

Trip::Trip(int id) :
    id(id),
    lineName("Line " + QString::number(id)),
    lastTime(QTime(0, 0))
{

}

Trip::Trip(int id, vector<Street_dir> route) : Trip (id)
{
    lineRoute = std::move(route);
    initStopsPositions();
}

Trip::Trip(int id, std::vector<Street_dir> route, std::vector<QTime> departures) : Trip(id, std::move(route))
{
    this->departures = std::move(departures);
    initStopsPositions();
}

int Trip::Id() const
{
    return id;
}

QString Trip::name() const
{
    return lineName;
}

vector<Street_dir> Trip::route() const
{
    return lineRoute;
}

vector<shared_ptr<Vehicle>> Trip::vehicles() const
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
    lastTime = QTime(time);
}

void Trip::advanceVehicleRoute(const std::shared_ptr<Vehicle>& v)
{
    v->restMSecs = RandomInRange(wait_min, wait_max);
    v->internal_street_index++;
    if (v->internal_street_index < lineRoute.size()) {
        Street_dir sd = lineRoute.at(v->internal_street_index);
        v->street = &std::get<0>(sd);
        v->direction = std::get<1>(sd);
        v->progress = 0;
    }
    else {
        // invalidating vehicle
        v->invalidate();
    }

    initStopsPositions();
}

void Trip::updateVehiclesAt(QTime time)
{
    if (vehiclePool.empty())
        return;

    // updating existing vehicles
    const int msecsElapsed = abs(time.msecsTo(lastTime));
    for (auto& vehicle : vehiclePool) {
        if (vehicle->isinvalid())
            continue;
        updateVehiclePosition(vehicle, msecsElapsed);

        // we've reached the end of the street
        double w; // wasted progress
        while (vehicle->internal_street_index < lineRoute.size() &&
               (w = vehicle->progress - lineRoute.at(vehicle->internal_street_index).first.time_cost) > 0) {
            advanceVehicleRoute(vehicle);

            w = vehicle->fromProgressToMSecs(w); // convert to wasted milliseconds

            updateVehiclePosition(vehicle, w); // advance again on new street
        }
    }
}

void Trip::updateVehiclePosition(const std::shared_ptr<Vehicle>& v, double elapsedMSecs)
{
    if (elapsedMSecs == 0)
        return;

    const qreal traffic_multiplier = (100.0-v->street->trafficDensity())/100.0;
    v->progress += (v->fromMSecsToProgress(elapsedMSecs) * traffic_multiplier);

    // begin waiting if the vehicle comes across a stop
    if (stopsPositions.at(v->internal_street_index) != -1 && v->progress > stopsPositions.at(v->internal_street_index)) {
        // waiting on a bus stop
        double waitedProgress = v->progress - stopsPositions.at(v->internal_street_index);
        v->progress = stopsPositions.at(v->internal_street_index);
        v->restMSecs -= v->fromProgressToMSecs(waitedProgress);
        if (v->restMSecs < 0) {
            if (v->internal_street_index + 2 == stopsPositions.size()) {
                // if the vehicle is on the last stop (stopsPositions is one item bigger)
                v->invalidate();
            }
            else {
                v->progress += v->fromMSecsToProgress(fabs(v->restMSecs));// * multiplier; // will introduce a weird bug: vehicles reverse back
            }
            v->restMSecs = 0;
        }
    }
}


std::vector<std::shared_ptr<Vehicle>> Trip::createNewVehiclesAt(QTime time)
{
    std::vector<std::shared_ptr<Vehicle>> new_vehicles {};
    if (departures.empty() || lineRoute.empty())
        return new_vehicles;

    for (auto t : departures)
        if (lastTime < t && t <= time) {
            std::shared_ptr<Vehicle> vehicle = std::make_shared<Vehicle>(this, t);
            vehicle->restMSecs = RandomInRange(wait_min, wait_max);
            vehiclePool.push_back(vehicle);
            new_vehicles.push_back(vehicle);
        }
    return new_vehicles;
}

std::vector<double> Trip::StopsPositions()
{
    return stopsPositions;
}


void Trip::initStopsPositions()
{
    stopsPositions.clear();
    for (size_t i = 0; i < lineRoute.size(); i++) {
        if (lineRoute.at(i).first.stops.empty()) {
            // fill with placeholders since this street has no bus stops
            stopsPositions.push_back(-1);
            continue;
        }

        double value =
            lineRoute.at(i).first.stops.at(0).street_percentage / 100 * lineRoute.at(i).first.time_cost;
        if (lineRoute.at(i).second == DIRBACKWARD)
            value = lineRoute.at(i).first.time_cost - value;
        stopsPositions.push_back(value);
    }
    stopsPositions.push_back(-1);
}
