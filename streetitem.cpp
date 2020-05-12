#include "streetitem.h"
#include "util.h"
#include "street.h"
#include <QDebug>
#include <QFont>

StreetItem::StreetItem(QLineF _line, QString _street_name, QGraphicsItem *parent) :
    QGraphicsLineItem(parent),
    street(nullptr),
    label(QGraphicsSimpleTextItem(_street_name, this))
{
    setLine(_line);
    name = _street_name;
    is_selected = false;
    is_closed = false;

    setPen(color_default());
    setFlag(QGraphicsItem::ItemIsSelectable);

    label.setFont(font_label());

    SetLabelPosition();
}

StreetItem::StreetItem(Street* _street, QGraphicsItem * parent) :
    StreetItem({*_street->point1, *_street->point2}, _street->name + "-" + QString::number(_street->id), parent)
{
    street = _street;
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
    auto aligned_rect = CenteredRectToRect(label.boundingRect(), text_center);

    label.setPos(aligned_rect.topLeft());

    // rotate around its center
    label.setTransformOriginPoint(label.boundingRect().center());

    // rotate to the line's orientation
    label.setRotation(-line().angle());
}

void StreetItem::SetLineWidth(int value)
{
    line_width = 1 + (value > 1 ?  qRound(log2(value)) : 0);
}


void StreetItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    // Label
    label.paint(painter, option, widget);

    // Line
    if (!is_closed && !is_selected)
    {
        //setPen(color_default());
        setPen(color_traffic(street ? street->trafficDensity() : 0));
    }
    else if(is_closed && !is_selected)
    {
        setPen(color_closed());
    }
    else if(is_closed && !is_selected)
    {
        setPen(color_closed());
    }
    else // closed and highlighted
    {
        setPen(color_closed_and_highlighted());
    }

    QGraphicsLineItem::paint(painter, option, widget);
}
