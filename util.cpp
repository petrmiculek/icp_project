#include "util.h"
#include "street.h"

#include <QPen>
#include <QRectF>
#include <cmath>

double euclid_distance(QPointF * point1, QPointF * point2)
{
    return sqrt(pow(point1->x() - point2->x(), 2) + pow(point1->y() - point2->y(), 2));
}


QRectF CenterRectToPoint(QRectF rect, QPointF point)
{
    auto top_left_x = point.x() - rect.width()/2;
    auto top_left_y = point.y() - rect.height()/2;

    return QRectF(top_left_x, top_left_y, rect.width(), rect.height());
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
    auto pens = std::vector<QPen>({
                                      {{Qt::red}, 1},
                                      {{Qt::blue}, 1},
                                      {{Qt::black}, 1},
                                      {{Qt::darkGreen}, 1},
                                      {{Qt::gray}, 1},
                                      {{Qt::darkBlue}, 1},
                                  });
    static int index = 0;

    return pens.at(index++ % pens.size());
}

QString toCamelCase(QString& s)
{
    QStringList parts = s.split(' ', QString::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join(" ");
}
