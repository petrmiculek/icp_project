#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_toggleTimeBtn_clicked()
{
    if (mapTimer->isRunning())
        mapTimer->stop();
    else
        mapTimer->start();

    updateTime();
}

void MainWindow::on_slowerBtn_clicked()
{
    const double currentMltplr = mapTimer->getMultiplier();
    mapTimer->setMultiplier(currentMltplr - 0.1);
}

void MainWindow::on_fasterBtn_clicked()
{
    const double currentMltplr = mapTimer->getMultiplier();
    mapTimer->setMultiplier(currentMltplr + 0.1);
}

void MainWindow::updateTime()
{
    const QString currentSpeed = QString::fromStdString(
                std::to_string(mapTimer->getMultiplier())).remove(3,10) + "x";
    const bool isRunning = mapTimer->isRunning();

    time_label->setText(mapTimer->currentTime("hh:mm:ss")
                      // workaround because can't get format "hh:mm:ss.z" to work properly
                      + mapTimer->currentTime(".z").remove(2,50));
    status_label->setText("(" + (isRunning?currentSpeed:"Stopped") + ")");
}
