#ifndef STOP_H
#define STOP_H

#include <QString>

class Stop
{
public:
    Stop(int id, double street_percentage, QString name);

    int id;
    double street_percentage; // fraction of distance from point1 to point2
    QString name;
};

#endif // STOP_H
