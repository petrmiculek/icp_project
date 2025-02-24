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

    /**
     * @brief Loads map data from files.
     * @return true on success, false on failure
     */
    bool LoadData();

    /**
     * @brief Loads JSON File of given structure, based on a file name.
     * @param file_name_prefix Prefix for a file path.
     * @param file_name File name.
     * @return true on success, false on failure
     */
    bool LoadJSONFile(const QString& file_name_prefix, const QString& file_name);

    std::vector<Street> streets;
    std::vector<Trip> trips;
};

#endif // DATAMODEL_H
