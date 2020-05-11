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

/**
 * @brief CenterRect Get rectangle with its center aligned to a specified point
 * @param rect rectangle to be aligned (only width-height are relevant)
 * @param point point to align to
 * @return QRectF aligned rectangle
 */
QRectF CenterRect(QRectF rect, QPointF point);

/**
 * @brief CenterRect Get Top-Left corner of rectangle with its center aligned to a specified point
 * @param rect rectangle to be aligned (only width-height are relevant)
 * @param point point to align to
 * @return QPointF Top-Left point of aligned rectangle
 */
QPointF CenterRectTopLeft(QRectF rect, QPointF point);

QPointF PositionOnLine(Street street, double street_percentage);
QPen NextColor();
QString toCamelCase(QString& s);

#endif // UTIL_H
