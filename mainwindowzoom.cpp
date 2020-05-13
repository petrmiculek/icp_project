#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trafficcircleitem.h"

#include <QPushButton>
#include <QResizeEvent>

static constexpr qreal scale_min = 2.5;
static constexpr qreal scale_max = 10.0;
// scale_default = 3.0


void MainWindow::SceneZoomIn()
{
    btn_zoom_out->setEnabled(true);

    auto scale_change = zoom_scale_factor;
    auto m = ui->graphicsView->transform();
    auto scale_old = m.m11(); // == m.m22(), scaling equally for horizontal and vertical

    auto scale_new = scale_old * scale_change;

    if(scale_new < scale_max) // OK, in range
    {
        TrafficCircleItem::scaling_ratio *= 1.0/scale_change; // INVERSE
    }
    else
    {
        scale_new = scale_max;

        TrafficCircleItem::scaling_ratio *= scale_old / scale_max;
        btn_zoom_in->setEnabled(false);
    }

    ui->graphicsView->setMatrix({scale_new, m.m12(), m.m21(), scale_new, m.dx(), m.dy()});
    scene->update();
}


void MainWindow::SceneZoomOut()
{
    btn_zoom_in->setEnabled(true);

    auto scale_change = 1.0/zoom_scale_factor; // INVERSE

    auto m = ui->graphicsView->transform();
    auto scale_old = m.m11(); // == m.m22(), scaling equally for horizontal and vertical

    auto scale_new = scale_old * scale_change;


    if(scale_new > scale_min) // OK, in range
    {
        TrafficCircleItem::scaling_ratio *= 1.0/scale_change; // INVERSE to INVERSE
    }
    else
    {
        scale_new = scale_min;
        TrafficCircleItem::scaling_ratio *= scale_old / scale_min;

        btn_zoom_out->setEnabled(false);
    }

    ui->graphicsView->setMatrix({scale_new, m.m12(), m.m21(), scale_new, m.dx(), m.dy()});
    scene->update();

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

    const auto button_size = 30; // h = w
    const auto base_offset = 15; // h = w
    const auto spacing = 3; // height of space between buttons

    parent->resize(base_offset + button_size, base_offset + 2 * button_size + spacing);
    parent->raise();

    btn_zoom_in = new QPushButton(parent);
    btn_zoom_in->setText("+");
    btn_zoom_in->setGeometry(base_offset, base_offset, button_size, button_size);

    btn_zoom_out = new QPushButton(parent);
    btn_zoom_out->setText("-");
    btn_zoom_out->setGeometry(base_offset, base_offset + button_size + spacing, button_size, button_size);

    QObject::connect(btn_zoom_in, &QPushButton::clicked, this, &MainWindow::ZoomInBtn_clicked);
    QObject::connect(btn_zoom_out, &QPushButton::clicked, this, &MainWindow::ZoomOutBtn_clicked);

    zoom_buttons_exist = true;
}
