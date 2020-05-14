#include <QDebug>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPoint>
#include <QXmlSimpleReader>
#include <QXmlInputSource>

#include <exception>
#include <cmath>

#include "stop.h"
#include "vehicle.h"

#include "datamodel.h"

/* datamodel.cpp
 * Project: CPP
 * Description: Data model and its setup
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

DataModel::DataModel(QObject *parent) : QObject(parent)
{
    auto res = LoadData();
    if(!res)
    {
        throw DataLoadingException(); // general exc
    }
}


bool DataModel::LoadData()
{
    static const QString files_prefix = "examples/";
    static const QString files[] = { "streets", "stops", "trips" };

    for (QString file : files)
        if (!LoadJSONFile(files_prefix, file))
            return false;

    return true;
}

/**
 * @brief DataModel::LoadJSONFile Load JSON File of given structure, based on file name
 * @param file_name
 * @return
 */
bool DataModel::LoadJSONFile(const QString file_name_prefix, const QString file_name)
{
    QFile file(":/" + file_name_prefix + file_name + ".json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "could not open file" << file_name;
        throw DataLoadingException();
    }

    QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));

    QJsonObject json(doc.object());

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
    }
    else if(file_name == "stops")
    {
        if (json.contains("stops") && json["stops"].isArray())
        {
            QJsonArray stops_json = json["stops"].toArray();

            for (int i = 0; i < stops_json.size(); ++i) {
                QJsonObject stop = stops_json[i].toObject();

                unsigned int street_id = stop["street_id"].toInt();
                if(street_id < streets.size()){
                    auto& street = streets.at(street_id);

                    int stop_id = stop["stop_id"].toInt();

                    double street_percentage = stop["street_percentage"].toDouble();

                    QString name = stop["name"].toString();
                    street.stops.emplace_back(stop_id, street_percentage, name);
                }
                else
                {
                    qDebug() << "stops: invalid street";
                    return false;
                }
            }
        }

    }
    else if(file_name == "trips")
    {
        if (streets.empty())
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

                QJsonArray streets_json = trip_json["streets_id"].toArray();
                std::vector<Street_dir> route;

                // streets
                for(auto street_dir : streets_json){
                    auto street_dir_arr = street_dir.toArray();

                    // street and direction
                    auto street_id = street_dir_arr[0].toInt();
                    auto dir = street_dir_arr[1].toInt();

                    // find if street exists
                    auto target_street = std::find_if(streets.begin(), streets.end(),
                                                      [street_id] (auto curr_street) {return curr_street.id == street_id;});

                    if (target_street == streets.end())
                    {
                        qDebug() << "trips: invalid street id" << street_id;
                        return false;
                    }

                    // street added to route
                    // (iterator to target street is dereferenced)
                    route.emplace_back(*target_street, dir);
                }

                // departure times
                std::vector<QTime> departures;

                QJsonArray departures_json = trip_json["departures"].toArray();
                for (auto departure_json : departures_json)
                {
                    QJsonArray departure_arr = departure_json.toArray();

                    auto departure_hours = departure_arr.at(0).toInt();
                    auto departure_minutes = departure_arr.at(1).toInt();
                    auto departure_seconds = departure_arr.at(2).toInt();

                    departures.emplace_back(departure_hours, departure_minutes, departure_seconds);
                }

                // save trip to collection
                trips.emplace_back(trip_id, route, departures);
            }
        }
    }

    return true;
}
