
#include "streetitem.h"
#include "util.h"
#include "street.h"
#include <QDebug>
#include <QFont>

StreetItem::StreetItem(QGraphicsItem *parent)
{
    StreetItem(QLineF(), "Sample", parent);
}

StreetItem::StreetItem(QLineF _line, QGraphicsItem *parent)
{

    StreetItem(_line, "Sample", parent);

}

StreetItem::StreetItem(QLineF _line, QString _street_name, QGraphicsItem *parent) :
    QGraphicsLineItem(parent),
    label(QGraphicsSimpleTextItem(_street_name, this))
{
    setLine(_line);
    name = _street_name;
    is_selected = false;
    is_closed = false;

    setPen(color_default);
    setFlag(QGraphicsItem::ItemIsSelectable);

    label.setPos(line().p1()); // temporary

    label.setFont(font_label);

    ComputeLabelPos();

}

StreetItem::StreetItem(Street street, QGraphicsItem * parent) :
    StreetItem({*street.point1, *street.point2}, street.name, parent)
{
    // nothing
}

QPointF StreetItem::ComputeLabelPos()
{

    QLineF normal = line().normalVector();
    auto center = line().center();
    auto tmp = line().p2() - center;
    normal.translate(tmp);
    normal.setLength(5);

    auto text_center = normal.p2();



    auto aligned_rect = CenterRectToPoint(label.boundingRect(), text_center);

    label.setPos(aligned_rect.topLeft());

    // label.setTransformOriginPoint(text_center);
    label.setRotation(-line().angle());

    return aligned_rect.topLeft();


    // get center point of line

    // shift point to top/right (so that it's not directly on the line)

    // compute top left corner of text


    // + (ensure text is above line, by text orientation?)
}

void StreetItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    // Label
    qDebug() << "" << label.text() << "Rotation:" <<label.rotation();


    label.paint(painter, option, widget);

    // Line
    if (!is_closed && !is_selected)
    {
        setPen(color_default);
    }
    else if(is_closed && !is_selected)
    {
        setPen(color_closed);
    }
    else if(is_closed && !is_selected)
    {
        setPen(color_closed);
    }
    else // closed and highlighted
    {
        setPen(color_closed_and_highlighted);
    }

    QGraphicsLineItem::paint(painter, option, widget);
}
