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
#include "datamodel.h"
#include "trafficcircleitem.h"
#include "mainwindow.h"

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
    mapTimer->setInterval(100); // setting refresh interval to 100 milliseconds
    QObject::connect(mapTimer, &MapTimer::timeout, this, &MainWindow::updateTime);
    assert(time_label = findChild<QLabel*>("timeLbl"));
    assert(status_label = findChild<QLabel*>("statusLbl"));

    initializeTimers();

    QObject::connect(ui->createRouteBtn, &QPushButton::clicked, this, &MainWindow::RouteCreateToggled);

    QObject::connect(scene, &QGraphicsScene::selectionChanged, this, &MainWindow::selectionChanged);

    selected_streets = {};
    selecting = false;
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
            TrafficCircleItem* scene_stop2 = new TrafficCircleItem(PositionOnLine(street, stop.street_percentage), bus_symbol);
            scene->addItem(scene_stop2);
        }
    }

    // vehicles

    // TODO

    connect(scene, &QGraphicsScene::selectionChanged,
            this, &MainWindow::selectionChanged);

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
