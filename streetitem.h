#ifndef STREETITEM_H
#define STREETITEM_H

#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QFont>
#include <algorithm>

#include "street.h"

class StreetItem : public QGraphicsLineItem
{
public:
    StreetItem(QLineF _line, QString _street_name, QGraphicsItem * parent = nullptr);    
    StreetItem(Street* street, QGraphicsItem * parent = nullptr);

    /**
     * @brief StreetItem::paint Reimplemented paint event handler - decide line color based on street state
     * @param painter see QGraphicsLineItem::paint
     * @param option see QGraphicsLineItem::paint
     * @param widget see QGraphicsLineItem::paint
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /**
     * @brief StreetItem::SetLabelPosition Align label (street name) next to the street
     */
    void SetLabelPosition();

    /**
     * @brief StreetItem::SetLineWidth Set line width based on the street's traffic density
     * @param traffic_density traffic density of the street
     */
    void SetLineWidth(int traffic_density);

    QString Name();
    void SetHighlight(bool highlighted);

    Street* GetStreet();

private:
    QString name;
    const Qt::GlobalColor default_color = Qt::lightGray;

    bool is_highlighted;
    bool is_closed;

    Street* street;

    QGraphicsSimpleTextItem label;

    int line_width = 1;

    inline QPen color_default()
    {
        return QPen(default_color, line_width);
    }

    inline QPen color_traffic(int traffic)
    {
        // use at least 15 % red shade with 10 % steps
        const float ratio = traffic ? std::max(0.15, std::round(traffic/10.0)/10.0) : 0;
        return QPen(MixColors(default_color, Qt::red, ratio), line_width);
    }

    inline QPen color_closed()
    {
        return QPen(Qt::red, line_width);
    }

    inline QPen color_highlighted()
    {
        return QPen(Qt::darkGray, line_width);
    }

    inline QPen color_closed_and_highlighted()
    {
        return QPen(Qt::darkRed, line_width);
    }

    inline QFont font_label()
    {
        return QFont("Helvetica", 2);
    }

    qreal distance_from_line_to_label = 3.0;
};


#endif // STREETITEM_H
