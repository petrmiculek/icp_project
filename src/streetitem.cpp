/* streetitem.cpp
 * Project: CPP
 * Description: StreetItem class - GUI representation of a street
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QDebug>
#include <QFont>

#include "street.h"
#include "util.h"
#include "streetitem.h"

qreal StreetItem::scaling_ratio = 1.0;

StreetItem::StreetItem(Street* _street, QGraphicsItem * parent) :
    QGraphicsLineItem({_street->point1, _street->point2},parent) // line
{
    street = _street;

#ifndef NDEBUG
    name = street->name + "-" + QString::number(_street->id);
#else
    name = street->name;
#endif

    setPen(Pen(default_color));
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    is_highlighted = false;

    label = new QGraphicsSimpleTextItem(street->name, (this)); // label

    label->setFont(FontLabel());

    SetLabelPosition();
}


void StreetItem::SetLabelPosition()
{

    // get center point of line
    auto center = line().center();

    // shift point to top/right (so that it's not directly on the line)
    auto normal = line().normalVector();
    auto shift_to_center = line().p2() - center;

    normal.translate(shift_to_center);

    // calculate length of the beforementioned shift, so that it's far enough
    normal.setLength(LabelDistance());
    auto text_center = normal.p2();

    // compute top left corner of text
    auto label_pos = CenteredRectToPoint(label->boundingRect(), text_center);

    label->setPos(label_pos);

    // rotate around its center
    label->setTransformOriginPoint(label->boundingRect().center());

    // rotate to the line's orientation
    label->setRotation(-line().angle());
}


QString StreetItem::Name()
{
    return name;
}


void StreetItem::SetHighlight(bool highlighted)
{
    is_highlighted = highlighted;
}


Street * StreetItem::GetStreet()
{
    if (street->id != Street().id) {
        return street;
    }
    else {
        return nullptr;
    }
}


void StreetItem::SetStreetTrafficDensity(int value)
{
    if(street)
    {
        street->setTrafficDensity(value);
    }
    else
    {
        qDebug() << "could not set traffic density - nullptr";
    }
}


void StreetItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    const float ratio = TrafficDensity(street->trafficDensity());
    label->setFont(FontLabel());

    SetLabelPosition();


    if (!is_highlighted) {
        //setPen(color_default());
        setPen(Pen(MixColors(default_color, default_traffic, ratio)));
    }
    else {
        setPen(Pen(MixColors(highlight_color, highlight_traffic, ratio)));
    }

    QGraphicsLineItem::paint(painter, option, widget);
}


qreal StreetItem::TrafficDensity(int traffic)
{
    // use at least 15 % red shade with 10 % steps
    return traffic ? std::max(0.15, std::round(traffic/10.0)/10.0) : 0.0;
}


QFont StreetItem::FontLabel()
{
    auto font = QFont("Helvetica", FontSize()); // truncates to int
    font.setPointSizeF(FontSize());
    return font;
}


QPen StreetItem::Pen(const QColor& color)
{
    return QPen(color, LineWidth());
}
