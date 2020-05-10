#include "trip.h"

#include <math.h>
#include <qdebug.h>

using namespace std;

Trip::Trip(QString name) : lineName(name), lastTime (nullptr)
{
    pen = NextColor();
}

Trip::Trip(QString name, vector<Street_dir> route) : lineName(name), lastTime (nullptr)
{
    this->lineRoute = route;
    pen = NextColor();
    initStopsPositions();
}

Trip::Trip(QString name, std::vector<Street_dir> route, std::vector<QTime> _departures) :
    departures(std::move(_departures)),
    lineName(name),
    lastTime(nullptr)
{
    lineRoute = route;
    pen = NextColor();
    initStopsPositions();
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
    v->restMSecs = rand() % 2500 + 750; // wait at least 750 msecs, max 3250 msecs
    v->internal_street_index++;
    if (v->internal_street_index < lineRoute.size()) {
        std::tie(v->street, v->direction) = lineRoute.at(v->internal_street_index);
        v->progress = 0;
    }
    else {
        // invalidating vehicle
        v->speed = SPEED_INVALID;
    }

    initStopsPositions();
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

    // begin waiting if the vehicle comes across a stop
    if (stopsPositions.at(v.internal_street_index) != -1 && v.progress > stopsPositions.at(v.internal_street_index)) {
        // waiting on a bus stop
        double waitedProgress = v.progress - stopsPositions.at(v.internal_street_index);
        v.progress = stopsPositions.at(v.internal_street_index);
        v.restMSecs -= v.fromProgressToMSecs(waitedProgress);
        if (v.restMSecs < 0) {
            v.progress += v.fromMSecsToProgress(fabs(v.restMSecs));
            v.restMSecs = 0;
        }
    }
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
                vehiclePool.push_back(Vehicle(lineRoute.front(), name(), pen, stopsPositions.front()));
                vehiclePool.back().restMSecs = rand() % 2500 + 750; // wait at least 750 msecs, max 3250 msecs
            }
    }
    else {
        // lastTime set => check if time is past this point
        for (auto t : departures)
            if (*lastTime < t && t <= time) {
                vehiclePool.push_back(Vehicle(lineRoute.front(), name(), pen, stopsPositions.front()));
                vehiclePool.back().restMSecs = rand() % 2500 + 750; // wait at least 750 msecs, max 3250 msecs
            }
    }
}

void Trip::initStopsPositions()
{
    stopsPositions.clear();
    for (size_t i = 0; i < lineRoute.size(); i++) {
        if (lineRoute.at(i).first.stops.size() == 0) {
            // fill with placeholders since this street has no bus stops
            stopsPositions.push_back(-1);
            continue;
        }

        double value =
            lineRoute.at(i).first.stops.front().street_percentage / 100 * lineRoute.at(i).first.time_cost;
        if (lineRoute.at(i).second == dir_backward)
            value = lineRoute.at(i).first.time_cost - value;
        stopsPositions.push_back(value);
    }
    stopsPositions.push_back(-1);
}


































