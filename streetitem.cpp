#include "streetitem.h"
#include "util.h"
#include "street.h"
#include <QDebug>
#include <QFont>

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

    label.setFont(font_label);

    SetLabelPosition();
}

StreetItem::StreetItem(Street street, QGraphicsItem * parent) :
    StreetItem({*street.point1, *street.point2}, street.name + "-" + QString::number(street.id), parent)
{
    // nothing
}

StreetItem::~StreetItem()
{
    qDebug() << "Street destroyed";
}

void StreetItem::SetLabelPosition()
{
    // get center point of line
    auto center = line().center();

    // shift point to top/right (so that it's not directly on the line)
    QLineF normal = line().normalVector();
    auto tmp = line().p2() - center;

    normal.translate(tmp);
    normal.setLength(3);

    auto text_center = normal.p2();

    // compute top left corner of text
    auto aligned_rect = CenterRectToPoint(label.boundingRect(), text_center);

    label.setPos(aligned_rect.topLeft());

    // rotate around its center
    label.setTransformOriginPoint(label.boundingRect().center());

    // rotate to the line's orientation
    label.setRotation(-line().angle());
}

void StreetItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    // Label
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
