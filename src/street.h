#ifndef STREET_H
#define STREET_H

/* vehicle.cpp
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

class Street
{
public:
    Street(int _id, int _x1, int _y1, int _x2, int _y2, QString _name) :
        id(_id),
        point1(new QPointF(_x1, _y1)),
        point2(new QPointF(_x2, _y2)),
        name(_name)
    {
        time_cost = euclid_distance(point1, point2);
    }

    Street(){}

    ~Street(){}

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

    int id{-1}; // if not initialized, must not collide with regular street IDs

    QPointF * point1{nullptr};
    QPointF * point2{nullptr};

    QString name{};

    std::vector<Stop> stops{};

    double time_cost;

private:
    int traffic_density{0}; // 0 = normal traffic
};

#endif // STREET_H
