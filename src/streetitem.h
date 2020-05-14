#ifndef STREETITEM_H
#define STREETITEM_H

/* streetitem.h
 * Project: CPP
 * Description: StreetItem class - GUI representation of a street
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QString>

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
       UNUSED
     * @brief StreetItem::SetLineWidth Set line width based on the street's traffic density
     * @param traffic_density traffic density of the street
     */
    // void SetLineWidth(int traffic_density);

    QString Name();
    void SetHighlight(bool highlighted);

    Street* GetStreet();

    void SetStreetTrafficDensity(int value);

    static constexpr Qt::GlobalColor default_color = Qt::lightGray;
    static constexpr Qt::GlobalColor highlight_color = Qt::darkGray;
    static constexpr Qt::GlobalColor default_traffic = Qt::red;
    static constexpr Qt::GlobalColor highlight_traffic = Qt::darkRed;

    qreal FontSize()
    {
        return font_size * scaling_ratio;
    }
    qreal LineWidth()
    {
        return line_width * scaling_ratio;
    }

    qreal LabelDistance()
    {
        auto value = (line_width/2.0 + label_distance) * scaling_ratio;
        return value;
    }

    static void Scale(qreal factor)
    {
        scaling_ratio *= factor;
    }

private:
    QString name;

    bool is_highlighted;

    Street* street;

    QGraphicsSimpleTextItem * label;
    QPointF text_center; // verify that it's needed

    static constexpr qreal line_width = 6.0;
    static constexpr qreal font_size = 9.0;

    QPen Pen(const QColor& color = default_color);
    qreal TrafficDensity(int traffic);
    QFont FontLabel();

    qreal label_distance = 12.0; // from street

    static qreal scaling_ratio; // 1.0 by default
};


#endif // STREETITEM_H
