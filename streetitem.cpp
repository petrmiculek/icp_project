#include "streetitem.h"
#include "util.h"
#include "street.h"
#include <QDebug>
#include <QFont>


StreetItem::StreetItem(QLineF _line, QString _street_name, QGraphicsItem *parent) :
    QGraphicsLineItem(parent),
    name(_street_name),
    is_highlighted(false),
    street(nullptr),
    label(QGraphicsSimpleTextItem(_street_name, this))
{
    setLine(_line);
    setPen(get_pen(default_color));
    setFlag(QGraphicsItem::ItemIsSelectable);

    label.setFont(font_label());
    SetLabelPosition();
}

StreetItem::StreetItem(Street* _street, QGraphicsItem * parent) :
    StreetItem({*_street->point1, *_street->point2},
#ifndef NDEBUG
    _street->name + "-" + QString::number(_street->id),
#else
    _street->name,
#endif
    parent)
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

    // calculate length of the beforementioned shift, so that it's far enough (street's thickness may vary)
    normal.setLength(line_width * 2.0/3 + distance_from_line_to_label);
    auto text_center = normal.p2();

    // compute top left corner of text
    auto aligned_rect = CenteredRectToRect(label.boundingRect(), text_center);

    label.setPos(aligned_rect.topLeft());

    // rotate around its center
    label.setTransformOriginPoint(label.boundingRect().center());

    // rotate to the line's orientation
    label.setRotation(-line().angle());
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
    if (street->id != Street().id) {
        return street;
    }
    else {
        return nullptr;
    }
}

void StreetItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    label.paint(painter, option, widget);
    const float ratio = traffic_ratio(street->trafficDensity());

    // Line
    if (!is_highlighted) {
        //setPen(color_default());
        setPen(get_pen(MixColors(default_color, default_traffic, ratio)));
    }
    else {
        setPen(get_pen(MixColors(highlight_color, highlight_traffic, ratio)));
    }

    QGraphicsLineItem::paint(painter, option, widget);
}

float StreetItem::traffic_ratio(int traffic)
{
    // use at least 15 % red shade with 10 % steps
    return traffic ? std::max(0.15, std::round(traffic/10.0)/10.0) : 0;
}

QFont StreetItem::font_label()
{
    return QFont("Helvetica", 2);
}

QPen StreetItem::get_pen(QColor color)
{
    return QPen(color, line_width);
}
