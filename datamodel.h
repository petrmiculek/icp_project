#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QIODevice>
#include <exception>

using direction = bool;

struct DataLoadingException : public std::exception {
   const char * what () const throw () {
      return "Custom";
   }
};

// crossroads / street endpoint
class Point
{
public:
    Point(int _x, int _y) : x(_x), y(_y) {}

private:
    // int id; // can be identified by position

    int x;
    int y;

};


class Stop
{
    int id;
    std::string name;
    int street_percentage; // fraction of distance from point1 to point2

};


class Street
{
public:
    Street(int _p1, int _p2, QString _name) : point_index1(_p1), point_index2(_p2), name(_name)
    {
        point1 = nullptr;
        point2 = nullptr;
    }

    // int id;

    Point * point1; // alternatively, index into points
    Point * point2;

    int point_index1;
    int point_index2;

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

    std::vector<Point> points;
    std::vector<Street> streets;
signals:

};

#endif // DATAMODEL_H
