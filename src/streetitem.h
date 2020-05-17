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
     * @brief Reimplemented paint event handler.
     * @details Decides line color based on street state.
     * @param painter Painter
     * @param option Options
     * @param widget Widget
     * @see QGraphicsLineItem::paint
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /**
     * @brief Aligns label (street name) next to the street.
     */
    void SetLabelPosition();

    /**
     * @brief Returns street name.
     * @return Street name.
     */
    QString Name();

    /**
     * @brief Sets the highlight flag for a street.
     * @param highlighted Flag
     */
    void SetHighlight(bool highlighted);

    /**
     * @brief Gets pointer to a valid street, or nullptr.
     * @return Street Pointer when the street is valid, nullptr if street is not valid/initialized.
     */
    Street* GetStreet();

    /**
     * @brief Sets traffic density to the street.
     * @param value Density value.
     * @remarks Value has to be in range 0 - 100 (incl.).
     */
    void SetStreetTrafficDensity(int value);

    // color scheme for streets
    static constexpr Qt::GlobalColor default_color = Qt::lightGray;
    static constexpr Qt::GlobalColor highlight_color = Qt::darkGray;
    static constexpr Qt::GlobalColor default_traffic = Qt::red;
    static constexpr Qt::GlobalColor highlight_traffic = Qt::darkRed;

    /**
     * @brief Gets scaled font size.
     * @return Font size scaled by zoom ratio.
     */
    qreal FontSize()
    {
        return font_size * scaling_ratio;
    }
    /**
     * @brief Gets scaled line width.
     * @return Line width scaled by zoom ratio.
     */
    qreal LineWidth()
    {
        return line_width * scaling_ratio;
    }

    /**
     * @brief Gets scaled line to label distance.
     * @return Distance scaled by zoom ratio.
     */
    qreal LabelDistance()
    {
        auto value = (line_width/2.0 + label_distance) * scaling_ratio;
        return value;
    }

    /**
     * @brief Scales items by a zoom factor.
     * @param Factor zoom factor (>1 -> zooming in, <1 zooming out).
     */
    static void Scale(qreal factor)
    {
        scaling_ratio *= factor;
    }

private:
    QPen Pen(const QColor& color = default_color);
    qreal TrafficDensity(int traffic);
    QFont FontLabel();

    QString name;

    bool is_highlighted;

    Street* street;

    QGraphicsSimpleTextItem * label;

    // base size properties of displayed line
    static constexpr qreal line_width = 6.0;
    static constexpr qreal font_size = 10.0;
    static constexpr qreal label_distance = 12.0; // from street

    static qreal scaling_ratio; // 1.0 by default
};


#endif // STREETITEM_H
