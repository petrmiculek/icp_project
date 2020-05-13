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

    static constexpr Qt::GlobalColor default_color = Qt::lightGray;
    static constexpr Qt::GlobalColor highlight_color = Qt::darkGray;
    static constexpr Qt::GlobalColor default_traffic = Qt::red;
    static constexpr Qt::GlobalColor highlight_traffic = Qt::darkRed;

private:
    QString name;

    bool is_highlighted;

    Street* street;

    QGraphicsSimpleTextItem * label;

    int line_width = 1;

    QPen get_pen(const QColor& color = default_color);
    qreal traffic_ratio(int traffic);
    QFont font_label();

    qreal distance_from_line_to_label = 3.0;
};


#endif // STREETITEM_H
