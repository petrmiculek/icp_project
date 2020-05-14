/* vehicle.h
 * Project: CPP
 * Description: Vehicle class holds internal vehicle data.
 * Author: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 */

#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>
#include <QTime>

#include "direction.h"
#include "street.h"
#include "util.h"

class Trip;

/**
 * @brief Value used to invalidate a vehicle.
 * @see Vehicle::invalidate
 * @see Vehicle::isinvalid
 */
#define INVALIDSPEED (-1)

/**
 * @brief Vehicle on a route.
 * @details Vehicle class holds internal information about a vehicle on a route.
 * It usually belongs to a Trip class instance.
 * @note A vehicle normally has to belong to a trip.
 * @see Trip
 */
class Vehicle
{
public:
    /**
     * @brief Creates new instance of Vehicle belonging to the trip.
     * @param trip Trip that the vehicle will belong to.
     * @param spawn_time Vehicle's spawn time.
     * @pre \p trip must be a valid instance of Trip class.
     */
    Vehicle(Trip *trip, QTime spawn_time);

    /**
     * @brief Trip the vehicle belongs to.
     */
    const Trip *trip;
    /**
     * @brief Vehicle's symbol.
     * @details Vehicle's symbol, usually a shortened version of line name.
     */
    const QString vehicle_symbol;

    /**
     * @brief QPen is used to draw vehicle on a map.
     */
    QPen pen;

    /**
     * @brief Current vehicle's street.
     */
    Street* street;
    /**
     * @brief Current vehicle's direction within the current street.
     */
    Direction direction;
    /**
     * @brief Internal route index.
     * @note This index makes sense only in scope of Trip class and is usually uninteresting for the user.
     * @see Trip::advanceVehicleRoute
     */
    unsigned int internal_street_index;
    /**
     * @brief Vehicle's progress within a street.
     */
    double progress;
    /**
     * @brief Vehicle's speed.
     */
    double speed;
    /**
     * @brief Time in milliseconds to be waited (i.e. at a bus stop).
     * @remark User manipulating this value will result in undefined behaviour.
     * @see Trip::updateVehiclePosition
     */
    double restMSecs; // milliseconds to rest (i.e. on a bus stop)

    /**
     * @brief Invalidates a vehicle (does not destroy it).
     * @see Vehicle::isinvalid
     */
    void invalidate();
    /**
     * @brief Checks if a vehicle is marked as invalid.
     * @return True if a vehicle is invalid.
     * @see Vehicle::invalidate
     */
    bool isinvalid() const;
    /**
     * @brief Returns in percentage where is a vehicle on a street.
     * @param street_cost Line length to compute the percentage from.
     * @return Position of a vehicle in percentage.
     * @pre \p street_cost must be equal or greater than zero.
     */
    double streetPercentage(double street_cost) const;
    /**
     * @brief Returns in percentage where is a vehicle on a street
     * @param street Street to compute the percentage from.
     * @return Position of a vehicle in percentage.
     * @pre \p street must be a valid pointer to a Street class instance.
     */
    double streetPercentage(const Street* street) const;
    /**
     * @brief Converts milliseconds to traveled distance by a vehicle.
     * @param msecs Time in milliseconds traveled by a vehicle.
     * @return Distance traveled by a vehicle in a given time.
     */
    double fromMSecsToProgress(double msecs);
    /**
     * @brief Converts traveled distance to time in milliseconds.
     * @details Converts distance to time in which a vehicle would travel such distance.
     * @param progress Traveled distance to be converted.
     * @return Time in milliseconds needed to travel given distance.
     */
    double fromProgressToMSecs(double progress);
    /**
     * @brief Returns vehicle's position.
     * @return vehicle's position
     */
    QPointF position() const;
    /**
     * @brief Returns the original spawn time.
     * @return spawn time
     */
    QTime spawnTime() const;
    /**
     * @brief Returns vehicle's symbol
     * @return Vehicle's symbol
     */
    QString symbol() const;

private:
    /**
     * @brief Creates new instance of Vehicle.
     * @param street_dir Starting street and direction.
     * @param line_name Vehicle's line name.
     * @param progress Starting street progress.
     * @param speed Speed of the vehicle.
     * @pre \p street_dir must be a valid instance of Street_dir.
     */
    Vehicle(Street_dir *street_dir, QString line_name, double progress = 0.0, double speed = speed_default);

    /**
     * @brief Spawn time of a vehicle.
     */
    QTime spawn_time;

protected:
    /**
     * @brief Default vehicle speed.
     */
    static constexpr double speed_default =
#ifndef NDEBUG
    0.075; // debug speed
#else
    0.003; // release speed
#endif
};

#endif // VEHICLE_H
