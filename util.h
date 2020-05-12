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
 * @param rect rectangle to be aligned (only width-height are used, position is ignored)
 * @param point point to align to
 * @return QRectF aligned rectangle
 */
QRectF CenteredRectToRect(QRectF rect, QPointF point);

/**
 * @brief CenterRect Get Top-Left corner of rectangle with its center aligned to a specified point
 * @param rect rectangle to be aligned (only width-height are relevant)
 * @param point point to align to
 * @return QPointF Top-Left point of aligned rectangle
 */
QPointF CenteredRectToPoint(QRectF rect, QPointF point);

/**
 * @brief CenteredSizeToRect Get rectangle with its center aligned to a specified point
 * @param dimensions size of desired rectangle
 * @param point point to align to
 * @return QRectF aligned rectangle
 */
QRectF CenteredSizeToRect(QSizeF dimensions, QPointF point);

/**
 * @brief PositionOnLine Get coordinates of element on a street
 * @param street Street that the element is on
 * @param street_percentage position of element within the street
 * @return QPointF element position as coordinates
 */
QPointF PositionOnLine(Street street, double street_percentage);


QColor NextColor(int index = -1); // index = -1  --> select random

QPen NextColorPen(int index = -1); // index = -1  --> select random

QColor MixColors(QColor c1, QColor c2, float ratio);

// https://wiki.qt.io/Converting_Strings_from_and_to_Camel_Case
QString toCamelCase(QString& s);

#endif // UTIL_H
