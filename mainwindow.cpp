#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QPen>

#include <assert.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "streetitem.h"
#include "util.h"

static constexpr uint a[] = {0x1F68D}; // bus Unicode symbol
static const QString bus_symbol = QString::fromUcs4(a,1);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    data = new DataModel();

    InitScene(data);
    AddZoomButtons();

    mapTimer = new MapTimer(0, 0, 0, 1.0, this);
    mapTimer->setInterval(50); // setting refresh interval to 100 milliseconds
    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::updateTime);
    QObject::connect(mapTimer, &MapTimer::reset_signal, this, &MainWindow::invalidateVehicles);
    time_label = findChild<QLabel*>("timeLbl");
    status_label = findChild<QLabel*>("statusLbl");
    assert(time_label);
    assert(status_label);

    initializeTimers();

    QObject::connect(ui->createRouteBtn, &QPushButton::clicked, this, &MainWindow::RouteCreateToggled);

    QObject::connect(scene, &QGraphicsScene::selectionChanged, this, &MainWindow::selectionChanged);

    // temporary
    selected_streets = {};
    selecting = false;

    initTrips();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete data;
    delete mapTimer;
    delete scene;
}


void MainWindow::selectionChanged()
{    
    QList<QGraphicsItem*> items = scene->selectedItems();

    if (items.size() != 1)
        return;

    // std::vector<Street> selectedStreets {};

    auto line = dynamic_cast<QGraphicsLineItem*>(items.first());
    for(auto street: data->streets)
    {
        auto pt1 = line->line().p1();
        auto pt2 = line->line().p2();

        if (pt1.x() == street.point1->x()
                && pt1.y() == street.point1->y()
                && pt2.x() == street.point2->x()
                && pt2.y() == street.point2->y())
        {
            selected_streets.push_back(street);
            // qDebug() << "found, dir1" << street.name;
            break;
        }
        else if (pt2.x() == street.point1->x()
                 && pt2.y() == street.point1->y()
                 && pt1.x() == street.point2->x()
                 && pt1.y() == street.point2->y())
        {
            selected_streets.push_back(street);
            // qDebug() << "found, dir2" << street.name;
            break;
        }
    }
}

void MainWindow::RouteCreateToggled()
{
    // toggle state
    selecting = ! selecting;

    if(selecting)
    {
        ui->createRouteBtn->setText(QString("Save route"));
        selected_streets.clear();
    }
    else
    {
        ui->createRouteBtn->setText(QString("Start creating route"));
        // create path from streets

        qDebug() << "Total streets selected:" << selected_streets.size();

        for (auto street : selected_streets) {
            qDebug() << street.id;
        }
    }
}

void MainWindow::InitScene(DataModel* data)
{
    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->scale(3, 3);

    // streets
    for (auto street : data->streets) {
        StreetItem* scene_street = new StreetItem(street);
        scene->addItem(scene_street);

        scene_streets.push_back(scene_street);
    }

    // stops
    for (auto street : data->streets) {
        for (auto stop: street.stops)
        {
            auto* scene_stop2 = new TrafficCircleItem(
                        PositionOnLine(street, stop.street_percentage), bus_symbol);
            scene->addItem(scene_stop2);
        }
    }

    connect(scene, &QGraphicsScene::selectionChanged,
            this, &MainWindow::selectionChanged);
}

void MainWindow::redrawVehicles(QTime time)
{
    deleteDrawnVehicles();

    for (auto& trip : data->trips) {
        trip.spawnVehiclesAt(time);
        for (auto& vehicle : trip.vehicles()) {
            if (vehicle.isinvalid())
                continue;

            auto* v = new TrafficCircleItem(
                        PositionOnLine(data->streets[vehicle.street_id],
                        vehicle.streetPercentage(data->streets[vehicle.street_id].time_cost)),
                        vehicle.symbol());
            scene->addItem(v);
            drawnVehicles.push_back(v);
        }
    }
}

void MainWindow::initTrips()
{
    for (auto& t : data->trips)
    {
        t.addSpawn(QTime(0,0,2));
    }

    // can connect to Trip's functions only through an intermediary as Trip is not a QObject
    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::redrawVehicles);
}

void MainWindow::invalidateVehicles()
{
    deleteDrawnVehicles();
    for (auto& trip : data->trips) {
        trip.setLastTime(mapTimer->currentTime());
        for (auto& vehicle : trip.vehicles())
            vehicle.invalidate();
    }
}

void MainWindow::deleteDrawnVehicles()
{
    for (size_t i = 0; i < drawnVehicles.size(); i++) {
        scene->removeItem(drawnVehicles[i]);
        delete drawnVehicles[i];
    }
    drawnVehicles.clear();
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
