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

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void SetLabelPosition();
    void SetLineWidth(int value);

    QString name;

    bool is_selected;
    bool is_closed;


    QGraphicsSimpleTextItem label;

    int line_width = 1;

    inline QPen color_default()
    {
        return QPen(Qt::lightGray, line_width);
    }

    inline QPen color_closed()
    {
        return QPen(Qt::red, line_width);
    }

    inline QPen color_highlighted()
    {
        return QPen(Qt::blue, line_width);
    }

    inline QPen color_closed_and_highlighted()
    {
        return QPen(Qt::darkRed, line_width);
    }

    inline QFont font_label()
    {
        return QFont("Helvetica", 2);
    }
};


#endif // STREETITEM_H
