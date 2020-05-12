#include "trafficcircleitem.h"
#include "util.h"

int TrafficCircleItem::scaling_ratio = 1;

// this constructor is not for vehicles
TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent) :
    TrafficCircleItem(center, content, NextColor(), nullptr, parent)
{
    // nothing
}

TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QPen _pen, std::shared_ptr<Vehicle> _vehicle, QGraphicsItem * parent) :
    QGraphicsEllipseItem(parent),
    text(content),
    pen(_pen),
    vehicle(_vehicle)
{
    MoveTo(center);

    // color
    setPen(pen);
    QColor color = pen.color();
    color.setAlpha(66);
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
    QGraphicsEllipseItem::paint(painter, option, widget);

    // text
    QFont font = painter->font();
    font.setPixelSize(7);
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

void TrafficCircleItem::SetRectCoords(QPointF point)
{
    QSizeF curr_rect_size = boundingRect().size();
    QRectF temp_rect = QRectF(point, curr_rect_size);
    setRect(temp_rect);
}
