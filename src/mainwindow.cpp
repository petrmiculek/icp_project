#include <memory>

#include <QAbstractItemModel>
#include <QDebug>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QHeaderView>
#include <QPen>
#include <QStandardItem>
#include <QTextStream>
#include <QWheelEvent>

#include "deselectabletreeview.h"
#include "streetitem.h"
#include "trafficcircleitem.h"
#include "ui_mainwindow.h"
#include "util.h"

#include "mainwindow.h"

/* mainwindow.cpp
 * Project: CPP
 * Description: MainWindow class - whole GUI
 * (mainwindow.h + mainwindow.cpp + mainwindowzoom.cpp + uilogic.cpp)
 *
 * Authors: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 *          Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    data = new DataModel();
    InitScene(data);
    AddZoomButtons();

    // initialize map timer
    mapTimer = new MapTimer(0, 0, 0, 1.0);
    mapTimer->setInterval(50); // default refresh interval
    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::updateTime);
    QObject::connect(mapTimer, &MapTimer::reset_signal, this, &MainWindow::invalidateVehicles);

    ui->pttreeView->window = this;

    // Selecting streets
    QObject::connect(scene, &QGraphicsScene::selectionChanged, this, &MainWindow::selectionChanged);
    // Selecting traffic density (slider)
    QObject::connect(ui->strttrafficSlider, &QSlider::valueChanged, this, &MainWindow::TrafficSliderChanged);
    // Choosing line from list
    QObject::connect(ui->pttreeView, &QTreeView::clicked, this, &MainWindow::ListSelectionChanged);
    // Redrawing vehicles when timer ticks
    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::redrawVehicles);

    initializeTimers();
    selected_streets = {};

#ifndef NDEBUG
    for (auto& t : data->trips) {
        t.addSpawn(QTime(0,0,1));
    }
#endif
}


MainWindow::~MainWindow()
{
    invalidateVehicles();
    delete ui;
    delete data;
    delete mapTimer;
    delete scene;
    delete treeViewModel;
}

void MainWindow::InitScene(DataModel* data)
{
    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

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
                        PositionOnLine(street, stop.street_percentage), stop.name.at(0));
            scene->addItem(scene_stop2);

        }
    }

    // initialize lines in tree view
    treeViewModel = new QStandardItemModel();
    for (const auto& trip : data->trips) {
        auto* lineItem = new QStandardItem(trip.name());
        for (const auto& street_dir : trip.route())
            for (const auto& stop : street_dir.first.stops)
                lineItem->appendRow(new QStandardItem(stop.name));
        treeViewModel->appendRow(lineItem);
    }
    ui->pttreeView->setModel(treeViewModel);
}


void MainWindow::redrawVehicles(QTime time)
{
    freeInvalidVehicles();

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

            // todo comment
            connect(v, &TrafficCircleItem::vehicleClicked,
                    this, &MainWindow::vehicleSelectionChanged);
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
    for (auto& trip : data->trips) {
        trip.setLastTime(mapTimer->currentTime());
        for (auto& vehicle : trip.vehicles())
            vehicle->invalidate();
    }
    freeInvalidVehicles();
}

void MainWindow::freeInvalidVehicles()
{
    for (size_t i = 0; i < drawnVehicles.size(); i++) {

        // only delete invalid
        if(drawnVehicles.at(i)->vehicle != nullptr
                && drawnVehicles.at(i)->vehicle->isinvalid())
        {
            scene->removeItem(drawnVehicles.at(i));
            delete drawnVehicles.at(i);
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

    int vehicles_selected_new = 0;

    // separate selected streets and vehicles
    for(const auto& item: items)
    {
        auto line = dynamic_cast<StreetItem*>(item);
        if (line != nullptr)
        {
            selected_streets.push_back(line);
        }

        auto vehicle = dynamic_cast<TrafficCircleItem*>(item);
        if (vehicle != nullptr)
        {
            vehicles_selected_new++;
        }
    }

    // if a line was highlighted through clicking on a vehicle
    // and no vehicle is highlighted anymore
    // remove the highlight
    if(vehicles_selected > 0
            && vehicles_selected_new == 0)
    {
        // de-highlight all
        for (auto& scene_street: scene_streets)
        {
            scene_street->SetHighlight(false);
        }
    }

    vehicles_selected = vehicles_selected_new;

    // no street is selected -> disable setting traffic density
    if (selected_streets.empty())
    {
        ui->strttrafficSlider->setEnabled(false);
        ui->strttrafficSlider->setValue(QSlider::NoTicks); // set slider to zero-value
        ui->strttrafficLbl->setEnabled(false);
        ui->strtnameLbl->clear();
        return;
    }

    // street selection is not empty => enable setting traffic density
    ui->strttrafficSlider->setEnabled(true);
    ui->strttrafficLbl->setEnabled(true);

    QString text;

    // display street name or the count of selected streets
    if(selected_streets.size() == 1)
    {
        text = selected_streets.at(0)->Name();
        ui->strttrafficSlider->setValue(selected_streets.at(0)->GetStreet()->trafficDensity()); // set slider to street's value

    }
    else
    {
        text = "More streets (" + QString::number(selected_streets.size()) + ")";
    }
    ui->strtnameLbl->setText(text);
}

void MainWindow::vehicleSelectionChanged(const Trip *trip)
{
    HighlightStreetsInTrip(trip);
}


void MainWindow::ListSelectionChanged(QModelIndex index)
{
    // clear all streets' highlight state
    for (auto& scene_street: scene_streets)
    {
        scene_street->SetHighlight(false);
    }

    if(ui->pttreeView->selectionModel()->selectedIndexes().size() != 1)
    {
        scene->update();
        return;
    }

    auto line_name = ui->pttreeView->model()->data(index);

    int index_found = -1;

    // find the selected trip
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
        scene->update();
        return;
    }

    HighlightStreetsInTrip(&data->trips.at(index_found));
}


void MainWindow::HighlightStreetsInTrip(const Trip * const trip)
{
    if(trip == nullptr)
        return;

    // clear all streets' highlight state
    for (auto& scene_street: scene_streets)
    {
        scene_street->SetHighlight(false);
    }

    for (const auto& street_dir : trip->route())
    {
        for (auto* const scene_street: scene_streets)
        {
            const auto* curr_street = scene_street->GetStreet();
            if(curr_street && street_dir.first.id == curr_street->id)
            {
                scene_street->SetHighlight(true);
            }
        }
    }

    scene->update();
}


void MainWindow::on_resettrafficBtn_clicked()
{
    ui->strttrafficSlider->setValue(QSlider::NoTicks);
    for (auto& street : data->streets) {
        street.setTrafficDensity(0);
    }
    scene->update();
}
