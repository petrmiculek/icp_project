#ifndef TRIP_H
#define TRIP_H

/* trip.h
 * Project: CPP
 * Description: Trip class represents a (bus) line - its route and departures
 * Author: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 */

#include <memory>

#include <QTime>

#include "direction.h"
#include "street.h"
#include "vehicle.h"

/**
 * @brief The Trip class holding information about a line and its vehicles.
 * @details The Trip class specifies line details such as line route.
 * It spawns vehicles on a route at given times and ensures a vehicle follows the route.
 */
class Trip
{
public:
    /**
     * @brief Instantiates new Trip class with empty line and spawn times.
     * @param id Line number
     */
    Trip(int id);
    /**
     * @brief Instantiates new Trip class with empty spawn times.
     * @param id Line number
     * @param route Line route
     */
    Trip(int id, std::vector<Street_dir> route);
    /**
     * @brief Instantiates new Trip class.
     * @param id Line number
     * @param route Line route
     * @param departures Departure times
     */
    Trip(int id, std::vector<Street_dir> route, std::vector<QTime> departures);

    /**
     * @brief Returns line number.
     * @return Line number
     */
    int Id() const;
    /**
     * @brief Returns line name.
     * @return Line name
     */
    QString name() const;
    /**
     * @brief Returns trip route.
     * @return Trip's route
     */
    std::vector<Street_dir> route() const;
    /**
     * @brief Returns vehicles owned by a trip.
     * @details Returns vehicles owned by a trip instance. This includes valid and invalid vehicles.
     * @return Vector of vehicles.
     * @see Vehicle::isinvalid
     */
    std::vector<std::shared_ptr<Vehicle>> vehicles() const;

    /**
     * @brief Adds street to the route.
     * @details Adds street to the route. Also specifies a direction the street
     * should be driven in.
     * @param s Street
     * @param d Direction
     */
    void addStreetToRoute(Street s, Direction d = DIRFORWARD);

    /**
     * @brief Adds spawn time to the line.
     * @details Adds new spawn time at which a vehicle should be spawned.
     * @param time Spawn time
     */
    void addSpawn(QTime time);

    /**
     * @brief Sets a last time a vehicle could have been spawned.
     * @details Last time should be set manually every time new vehicles are spawned and old vehicles are update.
     * @param time Last time
     */
    void setLastTime(QTime time);

    /**
     * @brief Updates vehicles until given time.
     * @details Elapsed time is determined by checking the given time against lastTime.
     * @param time Time to update vehicles up to.
     * @see Trip::setLastTime
     */
    void updateVehiclesAt(QTime time);

    /**
     * @brief Creates new vehicles.
     * @details Creates new vehicles that were to be spawned until given time.
     * @param time Time the vehicles will be spawned up to.
     * @return Vector of newly spawned vehicles.
     * @remarks New vehicles' starting position is on the route's first bus stop.
     * @see Trip::setLastTime
     */
    std::vector<std::shared_ptr<Vehicle>> createNewVehiclesAt(QTime time);

    /**
     * @brief Returns stops positions on the line's route.
     * @return Vector of positions
     * @see Trip::initStopsPositions
     */
    std::vector<double> StopsPositions();

private:
    static constexpr int wait_min = 750; //!< Minimum time in milliseconds a vehicle will wait on a bus stop.
    static constexpr int wait_max = 3250;//!< Maximum time in milliseconds a vehicle will wait on a bus stop.

    const int id; //!< Line number.
    const QString lineName; //!< Line name.

    /**
     * @brief Last time a vehicle pool was updated.
     * @remarks Last time has to be updated manually by a user.
     * @see Trip::setLastTime
     */
    QTime lastTime;
    std::vector<QTime> departures; //!< Line's spawn times.
    std::vector<Street_dir> lineRoute; //!< Line route.
    /**
     * @brief Stops positions of current line.
     * @see Trip::initStopsPositions
     */
    std::vector<double> stopsPositions;
    /**
     * @brief Pool of generated vehicles
     * @details Pool of trip's vehicles. Include both valid and invalid vehicles.
     * @see Vehicle::isinvalid
     * @see Trip::updateVehiclesAt
     * @see Trip::createNewVehiclesAt
     */
    std::vector<std::shared_ptr<Vehicle>> vehiclePool;

    /**
     * @brief Advances a vehicles position onto a next street.
     * @param v Vehicle's position to be updated.
     * @see Trip::updateVehiclePosition
     */
    void advanceVehicleRoute(const std::shared_ptr<Vehicle>& v);
    /**
     * @brief Updates vehicle's position
     * @param v Vehicle's position to be updated.
     * @param elapsedMSecs Time traveled by a vehicle.
     * @note If a vehicle reaches end of the street, it is passed onto the next one (no progress is discarded).
     * If a vehicle reaches a bus stop, elapsed time is used to wait. No progress is added.
     * When a vehicle reaches its final stop and has no wait time left, it is marked as invalid.
     * @remarks Elapsed time is converted to street's progress.
     * @see Vehicle::fromMSecsToProgress
     * @see Trip::advanceVehicleRoute
     */
    void updateVehiclePosition(const std::shared_ptr<Vehicle>& v, double elapsedMSecs);
    /**
     * @brief Initializes a vector of stops' positions on the current line.
     * @details Saves stops' positions into a vector as a cache. Takes into account the direction from which a vehicle approaches.
     * @remarks Stops positions should be updated by this function every time the line's route changes.
     */
    void initStopsPositions();
};

#endif // TRIP_H



















