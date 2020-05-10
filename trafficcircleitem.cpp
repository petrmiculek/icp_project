#include "trafficcircleitem.h"
#include "util.h"

TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent) :
    TrafficCircleItem(center, content, NextColor(), parent)
{

}

TrafficCircleItem::TrafficCircleItem(QPointF center, QString content, QPen pen, QGraphicsItem * parent) :
    QGraphicsEllipseItem(parent), text(content), pen(pen)
{
    QRectF rect = CenterRectToPoint(QRectF(center, center + point_ellipse_size), center);

    // circle bounding box
    setRect(rect);

    QPen pen_stop = pen;
    setPen(pen_stop);
    QColor color_stop = pen_stop.color();
    color_stop.setAlpha(66);
    setBrush(color_stop);

    // inner text bounding box
    text_space = QRectF(rect);
    text_space.setWidth(text_space.width() * inscribed_square_size);
    text_space.setHeight(text_space.height() * inscribed_square_size);
    text_space = CenterRectToPoint(text_space, center);
    text_space.translate(0, -1);

    // looking for something to keep buses/stops at a constant size on screen
    // (this does not do the trick)
    // setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void TrafficCircleItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QGraphicsEllipseItem::paint(painter, option, widget);

    QFont font = painter->font();
    font.setPixelSize(7);
    painter->setFont(font);
    painter->drawText(text_space, Qt::AlignCenter, text);
}
