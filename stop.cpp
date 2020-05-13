#include "stop.h"
#include "util.h"

Stop::Stop(int id, double street_percentage, QString name) :
    id(id),
    street_percentage(street_percentage)
{
    name = toCamelCase(name);
}
