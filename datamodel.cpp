#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "datamodel.h"

using direction = bool;

/* TODO
 * error handling -> exceptions (separate file?)
 *
 */

// sample route
void CreateTmpRoute(DataModel* data)
{
    // temporary
    std::vector<Street_dir> tmp_route;

    auto street_ids = {15, 16, 17, 18};

    for (auto s_id : street_ids)
    {
        bool found = false;
        for (Street street : data->streets)
        {
            if (street.id == s_id)
            {
                tmp_route.emplace_back(street, dir_default);
                found = true;
                break;
            }
        }

        if (! found)
        {
            qDebug() << "err creating route";
            return;
        }
    }

    Trip tmp_trip(QString("Test-linka"), tmp_route);

    data->trips.push_back(tmp_trip);
}


DataModel::DataModel(QObject *parent) : QObject(parent)
{
    auto res = LoadData();
    if(!res)
    {
        throw DataLoadingException(); // general exc
    }

    CreateTmpRoute(this);
}


bool DataModel::LoadData()
{
    bool res = LoadFile("streets");
    res &= LoadFile("stops");

    return res;
}


bool DataModel::LoadFile(QString file_name)
{
    QFile file(":/" + file_name + ".json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "could not open file" << file_name;
        throw DataLoadingException(); // "cannot open file"
    }

    QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));

    QJsonObject json(doc.object()); // top level item { .. }
    /*
    if(file_name == "points")
    {

        if (json.contains("points") && json["points"].isArray())
        {
            QJsonArray points_json = json["points"].toArray(); // "points": [ .. ]

            this->points.clear();

            // fill vector with data
            for (int i = 0; i < points_json.size(); ++i) {
                QJsonObject point = points_json[i].toObject(); // { "x": .. }

                // qDebug() << "json_points:" << point["point_id"] << point["x"] << point["y"];

                int x = point["x"].toInt();
                int y = point["y"].toInt();

                points.emplace_back(x, y);
            }
        }
        else
        {
            qDebug() << "points: main item not an array";
        }

    }
    else
    */
    if(file_name == "streets")
    {
        if (json.contains("streets") && json["streets"].isArray())
        {
            QJsonArray streets_json = json["streets"].toArray();

            this->streets.clear();

            // fill vector with data
            for (int i = 0; i < streets_json.size(); ++i) {
                QJsonObject street = streets_json[i].toObject();

                int start_x = street["start_x"].toInt();
                int start_y = street["start_y"].toInt();
                int end_x = street["end_x"].toInt();
                int end_y = street["end_y"].toInt();

                QString name = street["street_name"].toString();

                streets.emplace_back(i, start_x, start_y, end_x, end_y, name);

            }
        }
        else
        {
            qDebug() << "streets: main item not an array";
        }
    }
    else if(file_name == "stops")
    {

        /*
               "stop_id": 1,
               "street_id": 1,
               "street_percentage": 20,
               "name": "ABBEY LANE"
         */

        if (json.contains("stops") && json["stops"].isArray())
        {
            QJsonArray stops_json = json["stops"].toArray();

            for (int i = 0; i < stops_json.size(); ++i) {
                QJsonObject stop = stops_json[i].toObject();

                unsigned int street_id = stop["street_id"].toInt();
                if(street_id < streets.size()){
                    auto& street = streets.at(street_id);

                    int stop_id = stop["stop_id"].toInt();

                    int street_percentage = stop["street_percentage"].toInt();

                    QString name = stop["name"].toString();
                    street.stops.emplace_back(stop_id, street_percentage, name);
                }
                else
                {
                    qDebug() << "stops: invalid street";
                }
            }
        }
        else
        {
            qDebug() << "streets: main item not an array";
        }
    }
    else if(file_name == "trips")
    {
        if (streets.size() == 0)
        {
            qDebug() << "trips must be loaded after streets";
            return false;
        }

        if (json.contains("trips") && json["trips"].isArray())
        {
            QJsonArray stops_json = json["trips"].toArray();

            for (int i = 0; i < stops_json.size(); ++i)
            {
                QJsonObject trip_json = stops_json[i].toObject();

                unsigned int trip_id = trip_json["trip_id"].toInt();

                if(trip_json["streets_id"].isArray())
                {
                    QJsonArray streets_json = trip_json["streets_id"].toArray();
                    std::vector<Street_dir> route;

                    for(auto street : streets_json){
                        auto street_id = street.toInt();
                        auto target_street = std::find_if(streets.begin(), streets.end(),
                                                          [street_id] (Street curr_street) {return curr_street.id == street_id;});


                        if (target_street == streets.end())
                        {
                            qDebug() << "trips: invalid street id" << street_id;
                            return false;
                        }

                        // iterator to target street is dereferenced
                        route.emplace_back(*target_street, dir_default);

                    }

                    // save trip to collection

                    trips.emplace_back(QString::number(trip_id), route);
                }
                else
                {
                    qDebug() << "trips: invalid streets array";
                }

            }
        }
        else
        {
            qDebug() << "streets: main item not an array";
        }
    }


    return true;
}

