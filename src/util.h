/* util.h
 * Project: CPP
 * Description: Helpful utilities - positioning items, coloring items, miscellaneous
 * Authors: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 *          Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>

 */

#ifndef UTIL_H
#define UTIL_H

#include <QPen>
#include <QPointF>
#include <QRectF>

#include <cmath>
#include <exception>

class Street;

struct DataLoadingException : public std::exception {
   const char * what () const throw () {
      return "JSON data loading failed";
   }
};

/**
 * @brief Returns euclid distance of two points
 * @param point1 First point
 * @param point2 Second point
 * @return distance of two points
 */
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

/**
 * @brief Returns a color from the internal color list at the given index.
 * @remarks If the index is -1, returns the next color from the list.
 * @param index Index to return the color from.
 * @return color from the list at given index
 */
QColor NextColor(int index = -1); // index = -1  --> select random

QPen NextColorPen(int index = -1); // index = -1  --> select random

/**
 * @brief Mixes two colors in given ratio.
 * @param c1 First color.
 * @param c2 Second color.
 * @param ratio Mixing ratio. 0 means only the first color.
 * @return mixed color
 * @pre \p c1 and \p c2 must be valid colors.
 * @pre \p ratio must be in range [0,1].
 */
QColor MixColors(QColor c1, QColor c2, float ratio);

/**
 * @brief Converts given string to camel case. Spaces are preserved.
 * @param s String to be converted.
 * @return converted string
 */
QString ToCamelCase(QString& s);

/**
 * @brief Returns random integer from given range (inclusive).
 * @param min Minimum value.
 * @param max Maximum value.
 * @return Random integer in given range.
 * @pre \p min value must be less than \p max.
 * @remarks Function uses a pseudo-random number generator.
 * Generated values are not cryptographically safe.
 */
int RandomInRange(int min, int max);

#endif // UTIL_H
