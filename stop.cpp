#include "stop.h"
#include "util.h"

Stop::Stop(int _id, double _street_percentage, QString _name) :
    id(_id),
    street_percentage(_street_percentage)
{
    name = toCamelCase(_name);
}
