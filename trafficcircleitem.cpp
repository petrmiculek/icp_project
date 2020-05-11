#include "trafficcircleitem.h"
#include "util.h"

// this constructor is not for vehicles
TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent) :
    TrafficCircleItem(center, content, NextColor(), nullptr, parent)
{

}

TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QPen _pen, std::shared_ptr<Vehicle> _vehicle, QGraphicsItem * parent) :
    QGraphicsEllipseItem(parent),
    text(content),
    pen(_pen),
    vehicle(_vehicle)
{
    QRectF rect = CenterRect(QRectF(center, center + point_circle_size), center);

    // circle bounding box
    setRect(rect);

    setPen(pen);
    QColor color = pen.color();
    color.setAlpha(66);
    setBrush(color);

    // inner text bounding box
    text_space = QRectF(rect);
    text_space.setWidth(text_space.width() * inscribed_square_size);
    text_space.setHeight(text_space.height() * inscribed_square_size);
    text_space = CenterRect(text_space, center);
    text_space.translate(0, -1);
}

TrafficCircleItem::TrafficCircleItem(std::shared_ptr<Vehicle> _vehicle, QGraphicsItem *parent) :
    TrafficCircleItem(_vehicle->position(), _vehicle->symbol(), _vehicle->pen, _vehicle, parent)
  /*
      QGraphicsEllipseItem(parent),
      text(_vehicle->symbol()),
      pen(_vehicle->pen),
      vehicle (_vehicle)
          */
{
    //auto pos = vehicle->position();
}

void TrafficCircleItem::paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    /*
    // updating position here is not ideal
    if (vehicle != nullptr
            && vehicle->isinvalid() == false)
    {
        auto street_copy = vehicle->street;
        qDebug() << street_copy.name << vehicle->speed << vehicle->isinvalid() << vehicle->progress << vehicle->internal_street_index;

        MoveTo(vehicle->position());

    }
    */

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
    QPointF top_left = CenterRectTopLeft(boundingRect(), center);
    setRect({top_left, top_left + point_circle_size});


    // text label
    auto text_top_left = CenterRectTopLeft(text_space, center);
    text_space = QRectF(text_top_left, text_space.size());

}

void TrafficCircleItem::SetRectCoords(QPointF point)
{
    QSizeF curr_rect_size = boundingRect().size();
    QRectF temp_rect = QRectF(point, curr_rect_size);
    setRect(temp_rect);
}
