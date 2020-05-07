#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItem>
#include <QGraphicsScene>
#include <QGraphicsItem>

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

    // fooling around with gui, testing
    //QString content = QString::fromStdString(std::to_string(data->streets.size()) + " streets");
    //ui->label_json->setText(content);

    InitScene(data);

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

QPen next_color()
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

QPointF position_on_line(Street street, Stop stop)
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
        scene_street->setPen(next_color());

        scene_streets.push_back(scene_street);
    }

    // stops
    for (auto street : data->streets) {

        const static qreal stop_diameter = 10;
        const static auto point_ellipse_size = QPointF(stop_diameter, stop_diameter);

        for (auto stop: street.stops)
        {
            // offset so that ellipse center lies on the street
            QPointF top_left = position_on_line(street, stop) - point_ellipse_size/2;

            // ellipse bounding box
            auto ellipse_rect = QRectF(top_left, top_left + point_ellipse_size);

            QGraphicsEllipseItem* scene_stop = scene->addEllipse(ellipse_rect);
            scene_stop->setPen(next_color());

            scene_stops.push_back(scene_stop);
        }
    }

    // vehicles

    // TODO

}

/*
bool MainWindow::LoadData()
{
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(3);
    csvModel->setHorizontalHeaderLabels(QStringList());
    // ui->tableView->setModel(csvModel);

    auto header_names = QStringList();
    bool header_exists = false;

    QFile file(":/exampleTable.csv");
        if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
            qDebug() << "File not exists";
        } else {
            QTextStream in(&file);
            QString line;

            while (!in.atEnd())
            {
                line = in.readLine();
                // Adding to the model in line with the elements
                QList<QStandardItem *> standardItemsList;
                // consider that the line separated by semicolons into columns
                for (QString item : line.split(";")) {
                    if (header_exists)
                    {
                        standardItemsList.append(new QStandardItem(item));
                    }
                    else
                    {

                    }
                }
                csvModel->insertRow(csvModel->rowCount(), standardItemsList);
            }
            file.close();
        }

    return true;
}
*/
