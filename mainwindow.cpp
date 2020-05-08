#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>

#include <assert.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamodel.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto* data = new DataModel();

    InitScene(data);
    AddZoomButtons();

    mapTimer = new MapTimer(0, 0, 0, 1.0, this);
    mapTimer->setInterval(100); // setting refresh interval to 100 milliseconds
    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::updateTime);
    assert(time_label = findChild<QLabel*>("timeLbl"));
    assert(status_label = findChild<QLabel*>("statusLbl"));

    initializeTimers();
}


MainWindow::~MainWindow()
{
    delete ui;
}


QPen NextColor()
{
    auto pens = std::vector<QPen>({
                                      {{Qt::red}, 1},
                                      {{Qt::blue}, 1},
                                      {{Qt::black}, 1},
                                      {{Qt::darkGreen}, 1},
                                      {{Qt::gray}, 1},
                                      {{Qt::darkBlue}, 1},
                                  });

    static int index = 0;

    return pens.at(index++ % pens.size());
}


QRectF CenterRectToPoint(QRectF rect, QPointF point)
{
    auto top_left_x = point.x() - rect.width()/2;
    auto top_left_y = point.y() - rect.height()/2;

    return QRectF(top_left_x, top_left_y, rect.width(), rect.height());
}


QPointF PositionOnLine(Street street, int street_percentage)
{
    auto x_diff = street.point2->x() - street.point1->x();
    auto y_diff = street.point2->y() - street.point1->y();

    auto x = street.point1->x() + x_diff * street_percentage / 100.0;
    auto y = street.point1->y() + y_diff * street_percentage / 100.0;

    return QPointF(x, y);
}


class StopCircle :public QGraphicsEllipseItem
{
public:
    StopCircle(QPointF center)  // BUS: u8"\1F68D" -- doesn't work
    {
        QRectF rect = CenterRectToPoint(QRectF(center, center + point_ellipse_size), center);

        // circle bounding box
        setRect(rect);

        setPen(NextColor());
        setBrush({Qt::white});

        // inner text bounding box
        text_space = QRectF(rect);
        text_space.setWidth(text_space.width() * inscribed_square_size);
        text_space.setHeight(text_space.height() * inscribed_square_size);
        text_space = CenterRectToPoint(text_space, center);
        text_space.translate(0, -1);

        uint a[] = {0x1F68D};
        text = QString::fromUcs4(a,1);
    }

    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 ) override
    {
        QGraphicsEllipseItem::paint(painter, option, widget);

        QFont font = painter->font();
         font.setPixelSize(7);
         painter->setFont(font);
        painter->drawText(text_space, Qt::AlignCenter, text);    // Draw you text

    }

    // QGraphicsTextItem text;
    QRectF text_space;
    QString text;

private:
    // should be static
    static constexpr qreal stop_diameter = 10;
    static constexpr QPointF point_ellipse_size = {stop_diameter, stop_diameter};
    static constexpr qreal inscribed_square_size = 0.707;

};

void MainWindow::InitScene(DataModel* data)
{
    auto* scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->scale(2, 2);

    // streets
    for (auto street : data->streets) {
        auto qline = QLineF(*street.point1, *street.point2);

        QGraphicsLineItem* scene_street = scene->addLine(qline);
        scene_street->setPen(NextColor());
        scene_street->setFlag(QGraphicsItem::ItemIsSelectable);
        scene_streets.push_back(scene_street);
    }

    // stops
    for (auto street : data->streets) {
        for (auto stop: street.stops)
        {
            StopCircle* scene_stop2 = new StopCircle(PositionOnLine(street, stop.street_percentage));

            // QGraphicsEllipseItem* scene_stop = scene->addEllipse(ellipse_rect);
            // scene_stop->setPen(NextColor());
            // scene_stop->setBrush(QBrush(Qt::white));
            // scene_stops.push_back(scene_stop);

            scene->addItem(scene_stop2);
        }
    }

    // vehicles

    // TODO

}



/*
// podle videa https://www.youtube.com/watch?v=4dq7n8S9AHw
// udelat custom view, ktere nahradi tridu graphicsView?
void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier))
    {
        scaleView(pow((double)2, -event->delta() / 240.0));
    } else {
        QGraphicsView::wheelEvent(event);
    }
}
*/

void MainWindow::SceneZoomIn()
{
    ui->graphicsView->scale(zoom_scale_factor, zoom_scale_factor);
}

void MainWindow::SceneZoomOut()
{
    ui->graphicsView->scale(1/zoom_scale_factor, 1/zoom_scale_factor);

}

void MainWindow::ZoomInBtn_clicked()
{
    // add checks?
    // maximum zoom level
    SceneZoomIn();

}

void MainWindow::ZoomOutBtn_clicked(){
    // add checks?
    // minimum zoom level - when outer box of all objects gets too small or idk
    SceneZoomOut();
}

void MainWindow::AddZoomButtons()
{
    static bool zoom_buttons_exist = false;

    if (zoom_buttons_exist)
        return;

    QWidget* parent = new QWidget(this->ui->graphicsView);
    parent->resize(45, 80);
    parent->raise();

    const auto button_size = 30;

    auto* zoom_in = new QPushButton(parent);
    zoom_in->setText("+");
    zoom_in->setGeometry(15, 15, button_size, button_size);
    // zoom_in->raise();

    auto* zoom_out = new QPushButton(parent);
    zoom_out->setText("-");
    zoom_out->setGeometry(15, 15 + button_size + 3, button_size, button_size);
    // zoom_out->raise();

    QObject::connect(zoom_in, &QPushButton::clicked, this, &MainWindow::ZoomInBtn_clicked);
    QObject::connect(zoom_out, &QPushButton::clicked, this, &MainWindow::ZoomOutBtn_clicked);

    zoom_buttons_exist = true;
}
