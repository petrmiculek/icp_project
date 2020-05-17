/* trafficircleitem.cpp
 * Project: CPP
 * Description: GUI representation of a stop or a vehicle
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */
#include "util.h"
#include "trafficcircleitem.h"
#include "trip.h"

qreal TrafficCircleItem::scaling_ratio = 1.0;

// stops
TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent) :
    TrafficCircleItem(center, content, NextColorPen(), nullptr, parent)
{
    // nothing
}

// vehicles
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
        // stop
        color.setAlpha(90);
    }
    else
    {
        // vehicle
        color.setAlpha(30);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
    setBrush(color);
}

// vehicles
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

void TrafficCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(vehicle != nullptr)
    {
        emit vehicleClicked(vehicle->trip);
    }

    QGraphicsEllipseItem::mouseReleaseEvent(event);
}
