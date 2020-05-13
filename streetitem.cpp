#include "streetitem.h"
#include "util.h"
#include "street.h"
#include <QDebug>
#include <QFont>

StreetItem::StreetItem(Street* _street, QGraphicsItem * parent) :
    QGraphicsLineItem({*_street->point1, *_street->point2},parent) // line //
{
    street = _street;

#ifndef NDEBUG
    name = street->name + "-" + QString::number(_street->id);
#else
    name = street->name;
#endif

    setPen(get_pen(default_color));
    setFlag(QGraphicsItem::ItemIsSelectable);

    is_highlighted = false;

    label = new QGraphicsSimpleTextItem(street->name, (this)); // label

    label->setPos(line().center());
    label->setFont(font_label());
    SetLabelPosition();
}


void StreetItem::SetLabelPosition()
{
    // get center point of line
    auto center = line().center();

    // shift point to top/right (so that it's not directly on the line)
    QLineF normal = line().normalVector();
    auto tmp = line().p2() - center;

    normal.translate(tmp);

    // calculate length of the beforementioned shift, so that it's far enough (street's thickness may vary)
    normal.setLength(distance_from_line_to_label);
    auto text_center = normal.p2();

    // compute top left corner of text
    auto aligned_rect = CenteredRectToRect(label->boundingRect(), text_center);

    label->setPos(aligned_rect.topLeft());

    // rotate around its center
    label->setTransformOriginPoint(label->boundingRect().center());

    // rotate to the line's orientation
    label->setRotation(-line().angle());
}


void StreetItem::SetLineWidth(int traffic_density)
{
    line_width = 1 + (traffic_density > 1 ?  qRound(log2(traffic_density)) : 0);
    SetLabelPosition();
}

QString StreetItem::Name()
{
    return name;
}

void StreetItem::SetHighlight(bool highlighted)
{
    is_highlighted = highlighted;
}

/**
 * @brief StreetItem::GetStreet Get street if valid
 * @return Street pointer if the street is valid.
 *         Nullptr if the street is a default Street object
 */
Street * StreetItem::GetStreet()
{
    qDebug() << street->id;
    if (street->id != Street().id) {
        return street;
    }
    else {
        return nullptr;
    }
}

void StreetItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    const float ratio = traffic_ratio(street->trafficDensity());

    if (!is_highlighted) {
        //setPen(color_default());
        setPen(get_pen(MixColors(default_color, default_traffic, ratio)));
    }
    else {
        setPen(get_pen(MixColors(highlight_color, highlight_traffic, ratio)));
    }

    QGraphicsLineItem::paint(painter, option, widget);
}

qreal StreetItem::traffic_ratio(int traffic)
{
    // use at least 15 % red shade with 10 % steps
    return traffic ? std::max(0.15, std::round(traffic/10.0)/10.0) : 0.0;
}

QFont StreetItem::font_label()
{
    return QFont("Helvetica", 2);
}

QPen StreetItem::get_pen(const QColor& color)
{
    return QPen(color, line_width);
}
