#ifndef DATAMODEL_H
#define DATAMODEL_H

/* datamodel.h
 * Project: CPP
 * Description: Data model and its setup
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QJsonObject>
#include <QObject>
#include <QString>

#include "street.h"
#include "trip.h"

class DataModel : public QObject
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    bool LoadData();

    bool LoadJSONFile(const QString file_name_prefix, const QString file_name);
    void Read(const QJsonObject &json);

//private:
    std::vector<Street> streets;
    std::vector<Trip> trips;
};

#endif // DATAMODEL_H
