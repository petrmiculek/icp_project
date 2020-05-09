#ifndef STOP_H
#define STOP_H

#include <QString>

class Stop
{
public:
    Stop(int _id, double _street_percentage, QString _name);

    int id;
    double street_percentage; // fraction of distance from point1 to point2
    QString name;
};

#endif // STOP_H
