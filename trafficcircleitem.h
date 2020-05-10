#ifndef TRAFFICCIRCLEITEM_H
#define TRAFFICCIRCLEITEM_H

#include <QGraphicsEllipseItem>
#include <QPainter>
// #include "mainwindow.h"


class TrafficCircleItem : public QGraphicsEllipseItem
{
public:
    TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(QPointF center, QString content, QPen color, QGraphicsItem * parent = nullptr);

    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;

    // QGraphicsTextItem text;
    QRectF text_space;
    QString text;

    const QPen pen;

private:
    static constexpr qreal stop_diameter = 10;
    static constexpr QPointF point_ellipse_size = {stop_diameter, stop_diameter};
    static constexpr qreal inscribed_square_size = 0.707;
};

#endif // TRAFFICCIRCLEITEM_H
