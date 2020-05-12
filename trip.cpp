#include "trip.h"

#include <cmath>
#include <qdebug.h>

using namespace std;

constexpr int WAIT_MIN = 750;
constexpr int WAIT_MAX = 3250;
const QString line_name_prefix = "Line ";

Trip::Trip(int _id) :
    id(_id),
    lineName(line_name_prefix + QString::number(_id)),
    lastTime(nullptr)
{
}

Trip::Trip(int _id, vector<Street_dir> route) :
    id(_id),
    lineName(line_name_prefix + QString::number(_id)),
    lastTime(nullptr)
{
    lineRoute = route;
    initStopsPositions();
}

Trip::Trip(int _id, std::vector<Street_dir> route, std::vector<QTime> _departures) :
    id(_id),
    departures(std::move(_departures)), // TODO check if ok
    lineName(line_name_prefix + QString::number(_id)),
    lastTime(nullptr)
{
    lineRoute = route;
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
    lastTime = new QTime(time);
}

void Trip::advanceVehicleRoute(std::shared_ptr<Vehicle> v)
{
    v->restMSecs = rand() % (WAIT_MAX - WAIT_MIN) + WAIT_MIN;
    v->internal_street_index++;
    if (v->internal_street_index < lineRoute.size()) {
        Street_dir sd = lineRoute.at(v->internal_street_index);
        v->street = &std::get<0>(sd);
        v->direction = std::get<1>(sd);
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
            while (vehicle->internal_street_index < lineRoute.size() &&
                   (w=vehicle->progress - lineRoute.at(vehicle->internal_street_index).first.time_cost) > 0) {
                advanceVehicleRoute(vehicle);

                w = vehicle->fromProgressToMSecs(w); // convert to wasted milliseconds

                updateVehiclePosition(vehicle, w); // advance again on new street
            }
        }
    }
}

void Trip::updateVehiclePosition(std::shared_ptr<Vehicle> v, double elapsedMSecs)
{
    if (elapsedMSecs == 0)
        return;

    const float traffic_multiplier = (100.0-v->street->trafficDensity())/100.0;
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
            v->progress += v->fromMSecsToProgress(fabs(v->restMSecs));// * multiplier; // will introduce a weird bug when vehicles reverse back
            v->restMSecs = 0;
        }
    }
}


std::vector<std::shared_ptr<Vehicle>> Trip::createNewVehiclesAt(QTime time)
{
    std::vector<std::shared_ptr<Vehicle>> new_vehicles {};
    if (departures.size() == 0)
        return new_vehicles;

    // unpacking not needed currently, keeping for reference
    // auto& [street, Direction] = lineRoute.front();

    if (!lastTime) {
        // first call, lastTime was not set yet
        for (auto t : departures)
            if (t == time) {
                std::shared_ptr<Vehicle> vehicle = std::make_shared<Vehicle>(std::shared_ptr<Trip>(this));
                vehiclePool.push_back(vehicle);
                vehiclePool.back()->restMSecs = rand() % (WAIT_MAX - WAIT_MIN) + WAIT_MIN; // TODO Check if it does what it's supposed to
                new_vehicles.push_back(vehicle);
            }
    }
    else {
        // lastTime set => check if time is past this point
        for (auto t : departures)
            if (*lastTime < t && t <= time) {
                std::shared_ptr<Vehicle> vehicle = std::make_shared<Vehicle>(std::shared_ptr<Trip>(this)); //lineRoute.front(), name(), stopsPositions.front()
                vehiclePool.push_back(vehicle);
                vehiclePool.back()->restMSecs = rand() % (WAIT_MAX - WAIT_MIN) + WAIT_MIN;
                new_vehicles.push_back(vehicle);
            }
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


































