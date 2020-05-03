#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "datamodel.h"

/* TODO
 * error handling -> exceptions (separate file?)
 *
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
    bool res = LoadFile("points");
    res &= LoadFile("streets");

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

                points.emplace_back(Point(x, y));
            }
        }
        else
        {
            qDebug() << "points: main item not an array";
        }
    }
    else if(file_name == "streets")
    {
        if (json.contains("streets") && json["streets"].isArray())
        {
            QJsonArray streets_json = json["streets"].toArray();

            this->streets.clear();

            // qDebug() << "Streets: json.size() =" << json.size() << ", [\"points\"].toArray().size =" << streets_json.size();

            // fill vector with data
            for (int i = 0; i < streets_json.size(); ++i) {
                QJsonObject street = streets_json[i].toObject();

                // qDebug() << "json_points:" << point["point_id"] << point["x"] << point["y"];

                int point1 = street["point1_id"].toInt();
                int point2 = street["point2_id"].toInt();
                QString name = street["street_name"].toString();

                streets.emplace_back(Street(point1, point2, name));

            }
        }
        else
        {
            qDebug() << "streets: main item not an array";
        }
    }


    return true;
}

