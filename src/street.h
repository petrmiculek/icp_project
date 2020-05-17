#ifndef STREET_H
#define STREET_H

/* vehicle.h
 * Project: CPP
 * Description: Street class - coordinates, stops, name
 * (header only)
 * Authors: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 *          Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <assert.h>

#include <QPointF>
#include <QDebug>
#include <QString>

#include "util.h"
#include "stop.h"
#include "direction.h"

using Street_dir = std::pair<Street&, Direction>;

/**
 * @brief The Street class.
 * @details Street represents a part of Vehicle's route.
 * @see Vehicle
 */
class Street
{
public:
    /**
     * @brief Constructs Street class.
     * @param id Street's id.
     * @param x1 First point of the street on the x axis.
     * @param y1 First point of the street on the y axis.
     * @param x2 Second point of the street on the x axis.
     * @param y2 Second point of the street on the y axis.
     * @param Street's name.
     */
    Street(int id, int x1, int y1, int x2, int y2, QString name) :
        id(id),
        point1(QPointF(x1, y1)),
        point2(QPointF(x2, y2)),
        name(name),
        time_cost(euclid_distance(&point1, &point2))
    {

    }
    /**
     * @brief Construct's Street class.
     */
    Street():id(-1){}

    ~Street() {}

    /**
     * @brief trafficDensity get street traffic density
     * @return traffic density value <0; 100>
     */
    int trafficDensity() const
    {
        return traffic_density;
    }

    /**
     * @brief setTrafficDensity Set street's traffic density
     * @param value traffic density in the range <0; 100>
     */
    void setTrafficDensity(int value)
    {
        assert(value >= 0 && value <= 100);

        traffic_density = value;
    }

    /**
     * @brief Street's ID.
     */
    int id; // if not initialized, must not collide with regular street IDs

    QPointF point1; //!< First point of the street.
    QPointF point2; //!< Second point of the street.

    /**
     * @brief Street's name
     */
    QString name;

    /**
     * @brief List of stops on the street.
     */
    std::vector<Stop> stops;

    /**
     * @brief Time cost.
     * @details Time cost is computed as an euclid distance between starting and ending point of the street.
     * @see euclid_distance
     */
    double time_cost;

private:
    /**
     * @brief Traffic density.
     */
    int traffic_density{0}; // 0 = normal traffic
};

#endif // STREET_H
