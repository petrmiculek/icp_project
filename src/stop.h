#ifndef STOP_H
#define STOP_H

/* stop.h
 * Project: CPP
 * Description: Stop class
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QString>

/**
 * @brief The Stop class.
 * @details Public transport stop.
 */
class Stop
{
public:
    Stop(int id, double street_percentage, QString name);

    int id; //!< Stop ID.
    /**
     * @brief Position of a bus stop.
     */
    double street_percentage; // Fraction of distance from point1 to point2
    QString name; //!< Bus stop name.
};

#endif // STOP_H
