#ifndef TRAFFICCIRCLEITEM_H
#define TRAFFICCIRCLEITEM_H

/* trafficircleitem.h
 * Project: CPP
 * Description: GUI representation of a stop or a vehicle
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QGraphicsEllipseItem>
#include <QPainter>

#include "vehicle.h"

class TrafficCircleItem : public QGraphicsEllipseItem
{
public:
    TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(QPointF center, QString content, QPen color, std::shared_ptr<Vehicle> _vehicle, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(std::shared_ptr<Vehicle> _vehicle, QGraphicsItem * parent = nullptr);

    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;

    void MoveTo(QPointF center);

    void UpdateEllipseSize();

    static qreal CircleDiameter()
    {
        return circle_diameter * scaling_ratio;
    }

    static QSizeF PointCircleSize()
    {
        return {CircleDiameter(), CircleDiameter()};
    }

    static qreal TextSize()
    {
        return text_size_default * scaling_ratio;
    }

    QRectF text_space;

    const QPen pen;
    std::shared_ptr<Vehicle> vehicle;

    static qreal scaling_ratio;
private:
    QString text;
    static constexpr qreal circle_diameter = 36;

    static constexpr QSizeF point_circle_size = {circle_diameter, circle_diameter};

    // size of a square inside a circle, so that the square fits inside it
    static constexpr qreal inscribed_square_size_ratio = 0.707;

    static constexpr qreal text_size_default = circle_diameter / 2.0;
};

#endif // TRAFFICCIRCLEITEM_H
