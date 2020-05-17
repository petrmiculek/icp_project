/* mainwindowzoom.cpp
 * Project: CPP
 * Description: MainWindow class - Zoom-related part
 * (mainwindow.h + mainwindow.cpp + mainwindowzoom.cpp + uilogic.cpp)
 *
 * Author: Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trafficcircleitem.h"
#include "streetitem.h"

#include <QPushButton>
#include <QResizeEvent>

int MainWindow::zoom_current = 1;

void MainWindow::SceneZoomIn()
{
    zoom_current++;

    if(zoom_current == zoom_max)
    {
        btn_zoom_in->setEnabled(false);
    }

    btn_zoom_out->setEnabled(true);

    auto scale_change = zoom_scale_factor;
    auto m = ui->graphicsView->transform();
    auto scale_old = m.m11(); // == m.m22(), scaling equally for horizontal and vertical

    auto scale_new = scale_old * scale_change;

    ui->graphicsView->setMatrix({scale_new, m.m12(), m.m21(), scale_new, m.dx(), m.dy()});

    // drawn items scale inversely so as to keep a constant size on screen
    TrafficCircleItem::scaling_ratio *= 1.0/scale_change; // INVERSE
    StreetItem::Scale(1.0/scale_change); // INVERSE

    scene->update();
}


void MainWindow::SceneZoomOut()
{
    zoom_current--;

    if(zoom_current == zoom_min) // reached min zoom level
    {
        btn_zoom_out->setEnabled(false);
    }

    btn_zoom_in->setEnabled(true);

    auto scale_change = 1.0/zoom_scale_factor; // INVERSE

    auto m = ui->graphicsView->transform();
    auto scale_old = m.m11(); // == m.m22(), scaling equally for horizontal and vertical

    auto scale_new = scale_old * scale_change;

    ui->graphicsView->setMatrix({scale_new, m.m12(), m.m21(), scale_new, m.dx(), m.dy()});

    // drawn items scale inversely so as to keep a constant size on screen
    TrafficCircleItem::scaling_ratio *= 1.0/scale_change; // INVERSE to INVERSE
    StreetItem::Scale(1.0/scale_change); // INVERSE to INVERSE

    scene->update();
}


void MainWindow::ZoomInBtn_clicked()
{
    SceneZoomIn();
}


void MainWindow::ZoomOutBtn_clicked(){
    SceneZoomOut();
}


void MainWindow::AddZoomButtons()
{
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

    btn_zoom_out->setEnabled(false);
}
