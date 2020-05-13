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
#include "deselectabletreeview.h"
#include "trafficcircleitem.h"

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
    mapTimer->setInterval(50); // default refresh interval
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
        auto* lineItem = new QStandardItem(trip.name());
        for (const auto street_dir : trip.route())
            for (const auto& stop : street_dir.first.stops) {
                auto* stopItem = new QStandardItem(stop.name);
                lineItem->appendRow(stopItem);
            }
        model->appendRow(lineItem);
    }
    transport_tree_view->setModel(model);

    // Selecting streets
    QObject::connect(scene, &QGraphicsScene::selectionChanged, this, &MainWindow::selectionChanged);

    // Selecting traffic density (slider)
    QObject::connect(ui->strttrafficSlider, &QSlider::valueChanged, this, &MainWindow::TrafficSliderChanged);

    // Choosing line from list
    QObject::connect(transport_tree_view, &QTreeView::clicked, this, &MainWindow::ListSelectionChanged);

    initializeTimers();

    selected_streets = {};

    // TODO remove in final version
#ifndef NDEBUG
    for (auto& t : data->trips)
    {
        t.addSpawn(QTime(0,0,1));
    }
#endif

    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::redrawVehicles);
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
    for (auto& street : data->streets) {
        auto* scene_street = new StreetItem(&street);
        scene->addItem(scene_street);

        scene_streets.push_back(scene_street);
    }

    // stops
    for (const auto& street : data->streets) {
        for (const auto& stop: street.stops)
        {
            auto* scene_stop2 = new TrafficCircleItem(
                        PositionOnLine(street, stop.street_percentage), stop.name.front());
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
        for (const auto& vehicle : new_vehicles) {
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
    scene->update();
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
    for (auto& street: selected_streets)
    {
        street->SetStreetTrafficDensity(value);
    }

    scene->update();
}


void MainWindow::selectionChanged()
{
    QList<QGraphicsItem*> items = scene->selectedItems();

    selected_streets.clear();
    selected_street = NONE_SELECTED;

    if (items.empty())
    {
        ui->strttrafficSlider->setEnabled(false);
        ui->strttrafficSlider->setValue(QSlider::NoTicks); // set slider to zero-value
        ui->strttrafficLbl->setEnabled(false);
        ui->strtnameLbl->clear();
        return;
    }


    for(const auto& item: items)
    {
        auto line = dynamic_cast<StreetItem*>(item);
        if (line != nullptr)
        {
            selected_streets.push_back(line);
        }
    }

    ui->strttrafficSlider->setEnabled(true);
    ui->strttrafficLbl->setEnabled(true);

    QString text;

    if(selected_streets.size() == 1)
    {
        text = selected_streets.front()->Name();
        ui->strttrafficSlider->setValue(selected_streets.front()->GetStreet()->trafficDensity()); // set slider to street's value

    }
    else
    {
        text = "More streets(" + QString::number(selected_streets.size()) + ")";
    }
    ui->strtnameLbl->setText(text);


/*
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
            selected_streets.push_back(street);
            selected_street = static_cast<int>(i);

            ui->strttrafficSlider->setValue(street.trafficDensity());
            break;
        }
    }
    */
}


void MainWindow::ListSelectionChanged(QModelIndex index)
{
    // selected a line => clear highlight of all others
    for (auto& scene_street: scene_streets)
    {
        scene_street->SetHighlight(false);
    }

    if(ui->pttreeView->selectionModel()->selectedIndexes().size() != 1)
    {
        return;
    }

    auto line_name = ui->pttreeView->model()->data(index);

    int index_found = -1;
    for(unsigned long i = 0; i < data->trips.size(); i++)
    {
        auto& trip = data->trips.at(i);
        if (trip.name() == line_name)
        {
            index_found = i;
            break;
        }
    }

    if (index_found == -1)
    {
        return;
    }

    auto route = data->trips.at(index_found).route();

    // highlight all streets in selected line
    for (const auto& street_dir : route)
    {
        for (auto* const scene_street: scene_streets)
        {
            Street* curr_street = scene_street->GetStreet();
            if(curr_street && street_dir.first.id == curr_street->id)
            {
                scene_street->SetHighlight(true);
            }
        }
    }
    scene->update();

}
