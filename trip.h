#ifndef TRIP_H
#define TRIP_H

#include <QObject>

#include "datamodel.h"

using direction = bool;

class Trip : public QObject
{
    Q_OBJECT
public:
    explicit Trip(QString name, QObject *parent = nullptr);
    explicit Trip(QString name, std::vector<std::tuple<Street, direction>> route, QObject *parent = nullptr);

    QString name() const;
    std::vector<std::tuple<Street, direction>> route() const;

    void addStreetToRoute(Street s, direction d);

signals:
    void routeChanged();

private:
    QString lineName; // "N95"
    std::vector<std::tuple<Street, direction>> lineRoute;
};

#endif // TRIP_H
