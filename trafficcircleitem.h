#ifndef TRAFFICCIRCLEITEM_H
#define TRAFFICCIRCLEITEM_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include "vehicle.h"



class TrafficCircleItem : public QGraphicsEllipseItem
{
public:
    TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent = nullptr);
    // TrafficCircleItem(QPointF center, QString content, QPen color, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(QPointF center, QString content, QPen color, Vehicle * _vehicle, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(Vehicle * _vehicle, QGraphicsItem * parent = nullptr);

    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;
    void MoveTo(QPointF center);
    void SetRectCoords(QPointF point);


    // QGraphicsTextItem text;
    QRectF text_space;
    QString text;

    const QPen pen;
    Vehicle * vehicle;

private:
    static constexpr qreal stop_diameter = 10;
    static constexpr QPointF point_circle_size = {stop_diameter, stop_diameter};
    static constexpr qreal inscribed_square_size = 0.707;
};

#endif // TRAFFICCIRCLEITEM_H
