#include "util.h"
#include "street.h"

#include <QPen>
#include <QRectF>
#include <cmath>

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


QPointF PositionOnLine(Street street, double street_percentage)
{
    auto x_diff = street.point2->x() - street.point1->x();
    auto y_diff = street.point2->y() - street.point1->y();

    auto x = street.point1->x() + x_diff * street_percentage / 100.0;
    auto y = street.point1->y() + y_diff * street_percentage / 100.0;

    return QPointF(x, y);
}


QPen NextColor()
{
    // zero-thickness pen == cosmetic == constant width on screen (1pt)
    auto pens = std::vector<QPen>({
                                      {{Qt::darkCyan}, 0},
                                      {{Qt::blue}, 0},
                                      {{Qt::black}, 0},
                                      {{Qt::darkMagenta},0},
                                      {{Qt::darkGray}, 0},
                                      {{Qt::darkBlue}, 0},
                                  });
    static int index = 0;

    return pens.at(index++ % pens.size());
}

QColor MixColors(QColor c1, QColor c2, float ratio)
{
    if (ratio < 0 || ratio > 1)
        throw std::out_of_range("wrong color ratio");
    return QColor(
                c1.red() * (1 - ratio) + c2.red() * ratio,
                c1.green() * (1 - ratio) + c2.green() * ratio,
                c1.blue() * (1 - ratio) + c2.blue() * ratio
                );
}

QString toCamelCase(QString& s)
{
    QStringList parts = s.toLower().split(' ', QString::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join(" ");
}
