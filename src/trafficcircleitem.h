#ifndef TRAFFICCIRCLEITEM_H
#define TRAFFICCIRCLEITEM_H

/* trafficircleitem.h
 * Project: CPP
 * Description: GUI representation of a stop or a vehicle
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <memory>

#include <QGraphicsEllipseItem>
#include <QPainter>

#include "vehicle.h"

class TrafficCircleItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
signals:
    void vehicleClicked(const Trip * trip);

public:
    TrafficCircleItem(QPointF center, QString content, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(QPointF center, QString content, QPen color, std::shared_ptr<Vehicle> _vehicle, QGraphicsItem * parent = nullptr);
    TrafficCircleItem(std::shared_ptr<Vehicle> _vehicle, QGraphicsItem * parent = nullptr);

    /**
     * @brief paint Overridden QGraphicsEllipseItem::paint event
     * @param painter see QGraphicsEllipseItem::paint
     * @param option see QGraphicsEllipseItem::paint
     * @param widget see QGraphicsEllipseItem::paint
     *
     * @link https://doc.qt.io/qt-5/qgraphicsellipseitem.html#paint
     */
    void paint (QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0) override;

    /**
     * @brief MoveTo Move item to a point (which indicates item's center)
     * @param center Point that item will be centered to
     */
    void MoveTo(QPointF center);

    /**
     * @brief CircleDiameter Get item's scaled circle diameter
     * @return Circle diameter scaled by zoom ratio
     */
    static qreal CircleDiameter()
    {
        return circle_diameter * scaling_ratio;
    }

    /**
     * @brief PointCircleSize Get item's scaled dimensions
     * @return Circle dimensions scaled by zoom ratio
     */
    static QSizeF PointCircleSize()
    {
        return {CircleDiameter(), CircleDiameter()};
    }

    /**
     * @brief TextSize Get item's scaled text size
     * @return Text size scaled by zoom ratio
     */
    static qreal TextSize()
    {
        return text_size_default * scaling_ratio;
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QRectF text_space;

    const QPen pen;
    std::shared_ptr<Vehicle> vehicle;

    static qreal scaling_ratio;

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
