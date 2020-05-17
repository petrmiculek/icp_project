#ifndef TRAFFICCIRCLEITEM_H
#define TRAFFICCIRCLEITEM_H

/* trafficircleitem.h
 * Project: CPP
 * Description: GUI representation of a stop or a vehicle
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QGraphicsEllipseItem>
#include <QPainter>
#include <memory>


#include "vehicle.h"

class TrafficCircleItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
signals:
    /**
     * @brief vehicleClicked emitted when vehicle is clicked on/selected.
     * @param trip Trip of the target vehicle.
     */
    void vehicleClicked(const Trip * trip);

public:
    TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(QPointF center, QString content, QPen _pen, std::shared_ptr<Vehicle> _vehicle, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(const std::shared_ptr<Vehicle>& _vehicle, QGraphicsItem * parent = nullptr);

    /**
     * @brief Overridden QGraphicsEllipseItem::paint event.
     * @param painter Painter
     * @param option Option
     * @param widget Widget
     * @see QGraphicsEllipseItem::paint
     *
     * @link https://doc.qt.io/qt-5/qgraphicsellipseitem.html#paint
     */
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;

    /**
     * @brief Moves item to a point (which indicates item's center).
     * @param center Point that item will be centered to.
     */
    void MoveTo(QPointF center);

    /**
     * @brief Gets item's scaled circle diameter.
     * @return Circle diameter scaled by zoom ratio.
     */
    static qreal CircleDiameter()
    {
        return circle_diameter * scaling_ratio;
    }

    /**
     * @brief Gets item's scaled dimensions.
     * @return Circle dimensions scaled by zoom ratio.
     */
    static QSizeF PointCircleSize()
    {
        return {CircleDiameter(), CircleDiameter()};
    }

    /**
     * @brief Get item's scaled text size.
     * @return Text size scaled by zoom ratio.
     */
    static qreal TextSize()
    {
        return text_size_default * scaling_ratio;
    }

    /**
     * @brief Overriden QGraphicsItem::mouseReleaseEvent.
     * @param event Event data.
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QRectF text_space; //!< text_space bounding rectangle for text.

    const QPen pen; //!< Pen for drawing the TrafficCircleItem.

    std::shared_ptr<Vehicle> vehicle; //!< If TrafficCircleItem is a vehicle, points to its Vehicle object.

    static qreal scaling_ratio; //!< Current scaling ratio, affected by zoom level.

private:
    QString text;

    // base circle diameter for any TrafficCircleItem (stop or vehicle)
    static constexpr qreal circle_diameter = 36.0;

    // circle dimensions for any TrafficCircleItem (stop or vehicle)
    static constexpr QSizeF point_circle_size = {circle_diameter, circle_diameter};

    // size of a square inside a circle, so that the square fits inside it
    static constexpr qreal inscribed_square_size_ratio = 0.707;

    // base text size for text inside the TrafficCircleItem
    static constexpr qreal text_size_default = circle_diameter / 2.0;
};

#endif // TRAFFICCIRCLEITEM_H
