#define SMALLINCREMENT 0.1
#define BIGINCREMENT   1.0
#define MAXMULTIPLIER  100 // maximum multiplier value (absolute value)

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>

void MainWindow::on_toggleTimeBtn_clicked()
{
    if (mapTimer->isRunning())
        mapTimer->stop();
    else
        mapTimer->start();

    updateTime(); // update time to signal stopped status
}

void MainWindow::updateTime()
{
    QString currentSpeed = QString::fromStdString(
                std::to_string(mapTimer->getMultiplier()));
    if (mapTimer->getMultiplier() < 0)
        currentSpeed = currentSpeed.remove(4,100);
    else
        currentSpeed = currentSpeed.remove(3,100);

    const bool isRunning = mapTimer->isRunning();

    time_label->setText(mapTimer->currentTime("hh:mm:ss")
                      // workaround because can't get format "hh:mm:ss.z" to work properly
                      + mapTimer->currentTime(".z").remove(2,50));
    status_label->setText("(" + (isRunning?currentSpeed+"x":"Stopped") + ")");
}

void MainWindow::incrementMultiplier(const double increment)
{
    double currentMltplr = mapTimer->getMultiplier() + increment;

    if (fabs(currentMltplr) > MAXMULTIPLIER) {
        currentMltplr = currentMltplr < 0 ? -MAXMULTIPLIER : MAXMULTIPLIER;
    }

    mapTimer->setMultiplier(currentMltplr);
}

void MainWindow::on_slowerBtn_clicked()
{ incrementMultiplier(-SMALLINCREMENT); }
void MainWindow::on_fasterBtn_clicked()
{ incrementMultiplier(+SMALLINCREMENT); }
void MainWindow::on_lotfasterBtn_clicked()
{ incrementMultiplier(+BIGINCREMENT); }
void MainWindow::on_lotslowerBtn_clicked()
{ incrementMultiplier(-BIGINCREMENT); }
