#ifndef TRIP_H
#define TRIP_H

#include "street.h"

using direction = bool;

class Trip
{
public:
    Trip(QString name);
    Trip(QString name, std::vector<std::tuple<Street, direction>> route);

    QString name() const;
    std::vector<std::tuple<Street, direction>> route() const;

    void addStreetToRoute(Street s, direction d);

    // void routeChanged();

private:
    QString lineName; // "N95"
    std::vector<std::tuple<Street, direction>> lineRoute;
};

#endif // TRIP_H
