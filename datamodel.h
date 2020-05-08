#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QIODevice>
#include <exception>
#include <QPoint>
#include <math.h>

#include "street.h"
#include "trip.h"
#include "stop.h"
#include "vehicle.h"



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
