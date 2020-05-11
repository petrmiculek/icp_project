#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QPen>

#include <QAbstractItemModel>
#include <QHeaderView>

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

    // initialize map timer
    mapTimer = new MapTimer(0, 0, 0, 1.0, this);
    mapTimer->setInterval(50); // setting refresh interval to 50 milliseconds
    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::updateTime);
    QObject::connect(mapTimer, &MapTimer::reset_signal, this, &MainWindow::invalidateVehicles);

    // setup ui pointers
    time_label = ui->timeLbl;
    status_label = ui->statusLbl;
    transport_tree_view = ui->pttreeView;
    strttraffic_label = ui->strttrafficLbl;
    traffic_slider = ui->strttrafficSlider;

    // initialize lines in tree view
    auto* model = new QStandardItemModel();
    for (const auto& trip : data->trips) {
        auto* lineItem = new QStandardItem("Line " + trip.name());
        for (const auto street_dir : trip.route())
            for (const auto& stop : street_dir.first.stops) {
                auto* stopItem = new QStandardItem(stop.name);
                lineItem->appendRow(stopItem);
            }
        model->appendRow(lineItem);
    }
    transport_tree_view->setModel(model);

    // creating routes, deleted, keeping code for later - objížďky
    // QObject::connect(ui->createRouteBtn, &QPushButton::clicked, this, &MainWindow::RouteCreateToggled);

    // Selecting streets
    QObject::connect(scene, &QGraphicsScene::selectionChanged, this, &MainWindow::selectionChanged);

    // Selecting traffic density (slider)
    QObject::connect(ui->strttrafficSlider, &QSlider::valueChanged, this, &MainWindow::TrafficSliderChanged);

    // Choosing line from list
    QObject::connect(transport_tree_view, &QTreeView::clicked, this, &MainWindow::ListSelectionChanged);

    initializeTimers();

    // temporary
    selected_streets = {};
    selecting = false;

    initTrips(); // goes last
}


MainWindow::~MainWindow()
{
    delete ui;
    delete data;
    delete mapTimer;
    delete scene;
}

void MainWindow::InitScene(DataModel* data)
{
    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->scale(3, 3);

    // streets
    for (const auto& street : data->streets) {
        StreetItem* scene_street = new StreetItem(street);
        scene->addItem(scene_street);

        scene_streets.push_back(scene_street);
    }

    // stops
    for (const auto& street : data->streets) {
        for (const auto& stop: street.stops)
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

    // move existing + spawn new
    for (auto& trip : data->trips) {
        trip.updateVehiclesAt(time);
        std::vector<std::shared_ptr<Vehicle>> new_vehicles = trip.createNewVehiclesAt(time);
        trip.setLastTime(time);

        // create new vehicles
        for (auto vehicle : new_vehicles) {
            if (vehicle->isinvalid())
            {
                qDebug() << "redrawVehicles: invalid vehicle";
                continue;
            }
            auto* v = new TrafficCircleItem(vehicle);
            scene->addItem(v);
            drawnVehicles.push_back(v);
        }
    }

    // propagate changes to gui items
    for (auto* item : drawnVehicles) {

        if (item->vehicle == nullptr || item->vehicle->isinvalid())
        {
            continue;
        }

        item->MoveTo(item->vehicle->position());
    }
}

void MainWindow::initTrips()
{
    // TODO remove in final version
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
            vehicle->invalidate();
    }
}

void MainWindow::deleteDrawnVehicles()
{
    for (size_t i = 0; i < drawnVehicles.size(); i++) {

        // only delete invalid
        if(drawnVehicles.at(i)->vehicle != nullptr
                && drawnVehicles.at(i)->vehicle->isinvalid())
        {
            scene->removeItem(drawnVehicles.at(i));
            drawnVehicles.erase(drawnVehicles.begin() + i);
        }
    }
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

void MainWindow::TrafficSliderChanged(int value)
{
    if(selected_streets.size() != 1 || selected_street == NONE_SELECTED)
    {
        return;
    }

    // auto& street = selected_streets.front();  // unused

    auto& street = data->streets.at(selected_street);
    street.SetTrafficDensity(value);

    for (auto& str:scene_streets)
    {
        // find scene street to highlight accordingly
        auto pt1 = str->line().p1();
        auto pt2 = str->line().p2();

        if (pt1.x() == street.point1->x()
                && pt1.y() == street.point1->y()
                && pt2.x() == street.point2->x()
                && pt2.y() == street.point2->y())
        {
            str->SetLineWidth(value);
            scene->update();
            break;
        }
    }
}


void MainWindow::selectionChanged()
{
    QList<QGraphicsItem*> items = scene->selectedItems();

    selected_streets.clear();
    selected_street = NONE_SELECTED;

    if (items.size() != 1)
    {
        ui->strttrafficSlider->setEnabled(false);
        ui->strttrafficSlider->setValue(QSlider::NoTicks);
        ui->strttrafficLbl->setEnabled(false);
        return;
    }

    ui->strttrafficSlider->setEnabled(true);
    ui->strttrafficLbl->setEnabled(true);

    auto line = dynamic_cast<StreetItem*>(items.first());
    for(unsigned long i = 0; i < data->streets.size(); i++)
    {
        auto street = data->streets.at(i);

        auto pt1 = line->line().p1();
        auto pt2 = line->line().p2();

        if (pt1.x() == street.point1->x()
                && pt1.y() == street.point1->y()
                && pt2.x() == street.point2->x()
                && pt2.y() == street.point2->y())
        {
            selected_streets.push_back(street); // unused
            selected_street = static_cast<int>(i);

            ui->strttrafficSlider->setValue(qRound(street.trafficDensity()));
            break;
        }
    }


}



void MainWindow::ListSelectionChanged(QModelIndex index)
{

    if(ui->pttreeView->selectionModel()->selectedIndexes().size() != 1)
    {
        return;
    }

    auto line_name = ui->pttreeView->model()->data(index);

    int index_found = -1;
    for(unsigned int i = 0; i < data->trips.size(); i++)
    {
        auto trip = data->trips.at(i);
        if ("Line " + trip.name() == line_name)
        {
            index_found = i;
            break;
        }
    }

    if (index_found == -1)
    {
        return;
    }

    // selected a line => clear highlight of all others
    for (auto scene_street: scene_streets)
    {
        scene_street->is_selected = false;
    }

    auto route = data->trips.at(index_found).route();

    // highlight all streets in selected line
    for (const auto& street_dir : route)
    {
        for (auto* const scene_street: scene_streets)
        {
            if(street_dir.first.id == scene_street->street.id)
            {
                scene_street->is_selected = true;
            }
        }
    }
    scene->update();

}

/*
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
*/
