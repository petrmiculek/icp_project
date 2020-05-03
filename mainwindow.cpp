#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardItem>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto data = DataModel();

    data.LoadData();


    // fooling around with gui, testing
    ui->pushButton->setEnabled(false);
    QString content = QString::fromStdString(std::to_string(data.streets.size()));
    ui->label_json->setText(content);

}

MainWindow::~MainWindow()
{
    delete ui;

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
