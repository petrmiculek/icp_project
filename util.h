#ifndef UTIL_H
#define UTIL_H

#include <exception>
#include <QPointF>
#include <QPen>
#include <QRectF>
#include <cmath>

class Street;


struct DataLoadingException : public std::exception {
   const char * what () const throw () {
      return "Custom";
   }
};


double euclid_distance(QPointF * point1, QPointF * point2);
QRectF CenterRectToPoint(QRectF rect, QPointF point);
QPointF PositionOnLine(Street street, double street_percentage);
QPen NextColor();

#endif // UTIL_H
