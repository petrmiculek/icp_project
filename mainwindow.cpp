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
    mapTimer->setInterval(50); // setting refresh interval to 50 milliseconds
    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::updateTime);
    assert(time_label = findChild<QLabel*>("timeLbl"));
    assert(status_label = findChild<QLabel*>("statusLbl"));
}


MainWindow::~MainWindow()
{
    delete ui;
}


QPen NextColor()
{
    auto pens = std::vector<QPen>({
                 QPen({Qt::red}, 1),
                 QPen({Qt::blue}, 1),
                 QPen({Qt::black}, 1),
                 QPen({Qt::green}, 1)
    });

    static int index = 0;

    return pens.at(index++ % pens.size());
}


QPointF PositionOnLine(Street street, Stop stop)
{
    auto x_diff = street.point2->x() - street.point1->x();
    auto y_diff = street.point2->y() - street.point1->y();

    auto x = street.point1->x() + x_diff * stop.street_percentage / 100;
    auto y = street.point1->y() + y_diff * stop.street_percentage / 100;

    return QPointF(x, y);
}


void MainWindow::InitScene(DataModel* data)
{
    auto* scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

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

        const static qreal stop_diameter = 10;
        const static auto point_ellipse_size = QPointF(stop_diameter, stop_diameter);

        for (auto stop: street.stops)
        {
            // offset so that ellipse center lies on the street
            QPointF top_left = PositionOnLine(street, stop) - point_ellipse_size/2;

            // ellipse bounding box
            auto ellipse_rect = QRectF(top_left, top_left + point_ellipse_size);

            QGraphicsEllipseItem* scene_stop = scene->addEllipse(ellipse_rect);
            scene_stop->setPen(NextColor());

            scene_stops.push_back(scene_stop);
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
    parent->resize(400, 400);
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
