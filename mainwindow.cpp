#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItem>
#include <QGraphicsScene>

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
    ui->pushButton->setEnabled(false);
    QString content = QString::fromStdString(std::to_string(data->streets.size()) + " streets");
    ui->label_json->setText(content);

    qDebug() << "Hi";
    InitScene(data);

}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::InitScene(DataModel* data)
{
    auto* scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    std::vector<QLineF> lines;
    auto i = 0;
    for (auto line : data->streets) {

        auto scene_line = scene->addLine(line.point1->x(), line.point1->y(), line.point2->x(), line.point2->x());

    }

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
