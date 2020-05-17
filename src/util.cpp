/* util.cpp
 * Project: CPP
 * Description: Helpful utilities - positioning items, coloring items, miscellaneous
 * Authors: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 *          Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include "street.h"

#include "util.h"

double euclid_distance(QPointF * point1, QPointF * point2)
{
    return sqrt(pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2));
}


QRectF CenteredSizeToRect(QSizeF dimensions, QPointF point)
{
    auto top_left_x = point.x() - dimensions.width()/2.0;
    auto top_left_y = point.y() - dimensions.height()/2.0;

    return QRectF(top_left_x, top_left_y, dimensions.width(), dimensions.height());
}


QRectF CenteredRectToRect(QRectF rect, QPointF point)
{
    auto top_left_x = point.x() - rect.width()/2.0;
    auto top_left_y = point.y() - rect.height()/2.0;

    return QRectF(top_left_x, top_left_y, rect.width(), rect.height());
}


QPointF CenteredRectToPoint(QRectF rect, QPointF point)
{
    auto top_left_x = point.x() - rect.width()/2.0;
    auto top_left_y = point.y() - rect.height()/2.0;

    return QPointF(top_left_x, top_left_y);
}


QPointF PositionOnLine(const Street& street, double street_percentage)
{
    auto x_diff = street.point2.x() - street.point1.x();
    auto y_diff = street.point2.y() - street.point1.y();

    auto x = street.point1.x() + x_diff * street_percentage / 100.0;
    auto y = street.point1.y() + y_diff * street_percentage / 100.0;

    return QPointF(x, y);
}


QPen NextColorPen(int index) // index = -1  --> select random
{
    // zero-thickness pen == cosmetic == constant width on screen (1pt)
    static constexpr int pen_thickness = 0;

    return {NextColor(index), pen_thickness};

}

QColor MixColors(const QColor& c1, const QColor& c2, float ratio)
{
    if (ratio < 0 || ratio > 1)
        throw std::out_of_range("wrong color ratio");
    return QColor(
                static_cast<int>(c1.red() * (1 - ratio) + c2.red() * ratio),
                static_cast<int>(c1.green() * (1 - ratio) + c2.green() * ratio),
                static_cast<int>(c1.blue() * (1 - ratio) + c2.blue() * ratio)
                );
}

QString ToCamelCase(QString& s)
{
    // https://wiki.qt.io/Converting_Strings_from_and_to_Camel_Case
    QStringList parts = s.toLower().split(' ', QString::SkipEmptyParts);
    for (auto& part : parts)
        part.replace(0, 1, part[0].toUpper());

    return parts.join(" ");
}

QColor NextColor(int index)
{
    auto pens = std::vector<QColor>({
                                      {Qt::darkCyan},
                                      {Qt::blue},
                                      {Qt::black},
                                      {Qt::darkMagenta},
                                      {20, 100, 20},
                                      {Qt::darkBlue},
                                  });
    static int static_index = 0;

    if(index == -1)
    {
        // return random
        return pens.at(static_index++ % pens.size());
    }

    if (index < 0)
    {
        index = 0;
    }
    if (index >= static_cast<int>(pens.size()))
    {
        index = pens.size() - 1;
    }
    return pens.at(index);

}

int RandomInRange(int min, int max)
{
    return rand() % (max - min) + min;
}

QPointF Center(QLineF line)
{
    return QPointF(0.5 * line.p1() + 0.5 * line.p2());
}
