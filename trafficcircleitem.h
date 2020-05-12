#ifndef TRAFFICCIRCLEITEM_H
#define TRAFFICCIRCLEITEM_H

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
    QString text;

    const QPen pen;
    std::shared_ptr<Vehicle> vehicle;

    static qreal scaling_ratio;
private:
    static constexpr qreal circle_diameter = 12;

    static constexpr QSizeF point_circle_size = {circle_diameter, circle_diameter};

    // size of square inside circle, so that the square fits inside it
    static constexpr qreal inscribed_square_size_ratio = 0.707;

    static constexpr qreal text_size_default = circle_diameter / 2.0;
};

#endif // TRAFFICCIRCLEITEM_H
