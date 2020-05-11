#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>

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

    auto* parent = new QWidget(this->ui->graphicsView);
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
