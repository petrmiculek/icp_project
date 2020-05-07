#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QIODevice>
#include <exception>
#include <QPoint>
#include <math.h>

using direction = bool;

struct DataLoadingException : public std::exception {
   const char * what () const throw () {
      return "Custom";
   }
};
/*
// crossroads / street endpoint
class Point
{
public:
    Point(int _x, int _y) : x(_x), y(_y) {}

// private:
    // int id; // can be identified by position

    int x;
    int y;

};
*/

static inline double euclid_distance(QPointF * point1, QPointF * point2)
{
    return sqrt(pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2));
}

class Stop
{
    int id;
    std::string name;
    int street_percentage; // fraction of distance from point1 to point2
};


class Street
{
public:
    Street(int _id, int _x1, int _y1, int _x2, int _y2, QString _name) :
        id(_id),
        point1(new QPointF(_x1, _y1)),
        point2(new QPointF(_x2, _y2)),
        name(_name),
        stops({})     // todo do I have to initialize empty vector?
    {
        time_cost = euclid_distance(point1, point2);
    }

    int id;
    // static int max_street_id;

    QPointF * point1;
    QPointF * point2;

    QString name;

    std::vector<Stop> stops;

    int time_cost;


};


class Trip
{
    int id;
    std::string name; // "Bus N95"
    std::vector<std::tuple<Street, direction>> route;

};


class Vehicle
{
    int id;
    int street_id;  // which street it's on
    int street_percentage;
    Trip * trip;

};


class DataModel : public QObject
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    bool LoadData();

    bool LoadFile(QString file_name);
    void Read(const QJsonObject &json);

// private:

    std::vector<Street> streets;
signals:

};

#endif // DATAMODEL_H
