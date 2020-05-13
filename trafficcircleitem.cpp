#include "util.h"

#include "trafficcircleitem.h"

qreal TrafficCircleItem::scaling_ratio = 1.0;

// this constructor is not for vehicles
TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent) :
    TrafficCircleItem(center, content, NextColorPen(), nullptr, parent)
{
    // nothing
}

TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QPen _pen, std::shared_ptr<Vehicle> _vehicle, QGraphicsItem * parent) :
    QGraphicsEllipseItem(parent),
    pen(_pen),
    vehicle(_vehicle),
    text(content)
{
    MoveTo(center);

    // color
    setPen(pen);
    QColor color = pen.color();
    if(vehicle == nullptr)
    {
        color.setAlpha(90);
    }
    else
    {
        color.setAlpha(30);

    }
    setBrush(color);
}

TrafficCircleItem::TrafficCircleItem(std::shared_ptr<Vehicle> _vehicle, QGraphicsItem *parent) :
    TrafficCircleItem(_vehicle->position(), _vehicle->symbol(), _vehicle->pen, _vehicle, parent)

{
    // nothing
}

void TrafficCircleItem::paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    // circle
    if(vehicle != nullptr)
    {
        // vehicle
        MoveTo(vehicle->position());
    }
    else
    {
        // stop
        MoveTo(this->boundingRect().center());
    }
    QGraphicsEllipseItem::paint(painter, option, widget);

    // text
    QFont font = painter->font();
    font.setPointSizeF(TextSize());
    painter->setFont(font);

    painter->drawText(text_space, Qt::AlignCenter, text);
}

void TrafficCircleItem::MoveTo(QPointF center)
{
    // circle
    QRectF rect = CenteredSizeToRect(PointCircleSize(), center);

    setRect(rect);

    // text
    text_space = CenteredSizeToRect(PointCircleSize() * inscribed_square_size_ratio, center);
}

void TrafficCircleItem::UpdateEllipseSize()
{
    prepareGeometryChange();
    setRect({this->boundingRect().topLeft(), this->PointCircleSize()});
}


