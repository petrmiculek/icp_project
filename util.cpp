#include "util.h"

#include <cmath>

inline double euclid_distance(QPointF * point1, QPointF * point2)
{
    return sqrt(pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2));
}
