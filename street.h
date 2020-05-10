#ifndef STREET_H
#define STREET_H

#include <QPointF>
#include <QString>

#include "util.h"
#include "stop.h"

using Direction = bool;
using Street_dir = std::pair<Street&, Direction>; // TODO TEST CHANGE TO REFERENCE

class Street
{
public:
    Street(int _id, int _x1, int _y1, int _x2, int _y2, QString _name) :
        id(_id),
        point1(new QPointF(_x1, _y1)),
        point2(new QPointF(_x2, _y2)),
        name(_name),
        traffic_density(0)
    {
        time_cost = euclid_distance(point1, point2);
    }

    int id;

    QPointF * point1;
    QPointF * point2;

    QString name;

    std::vector<Stop> stops;

    double time_cost;
    double traffic_density;
};

#endif // STREET_H
