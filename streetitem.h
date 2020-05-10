#ifndef STREETITEM_H
#define STREETITEM_H

#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QFont>

class Street;

class StreetItem : public QGraphicsLineItem
{
public:
    StreetItem(QLineF _line, QString _street_name, QGraphicsItem * parent = nullptr);    
    StreetItem(Street street, QGraphicsItem * parent = nullptr);
    ~StreetItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void SetLabelPosition();

    QString name;

    bool is_selected;
    bool is_closed;

    QGraphicsSimpleTextItem label;

    // cannot get this to work as static
    const QPen color_default = QPen(Qt::lightGray);
    const QPen color_closed = QPen(Qt::red);
    const QPen color_highlighted = QPen(Qt::blue);
    const QPen color_closed_and_highlighted = QPen(Qt::darkRed);
    const QFont font_label = QFont("Helvetica", 2);
};


#endif // STREETITEM_H
