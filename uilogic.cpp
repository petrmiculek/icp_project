#define SMALLINCREMENT 0.1
#define BIGINCREMENT   1.0
#define MAXMULTIPLIER  100 // maximum multiplier value (absolute value)

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringRef>
#include <qdebug.h>

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
    // current multiplier
    const double multiplier = mapTimer->getMultiplier();
    // expected multiplier string length
    const size_t mltplr_length = std::to_string(int(truncl(multiplier))).length() + 2;
    // convert multiplier to string
    const QString string_multiplier = QString::fromStdString(std::to_string(multiplier));
    // remove unwanted decimal digits
    const QStringRef displayed_multiplier(&string_multiplier, 0, mltplr_length); // substring

    time_label->setText(mapTimer->currentTime("hh:mm:ss")
                      // workaround because can't get format "hh:mm:ss.z" to work properly
                      + mapTimer->currentTime(".z").remove(2,50));

    // returns current multiplier if the time is running, otherwise "Stopped"
    status_label->setText("(" + (mapTimer->isRunning()?displayed_multiplier.toString()+"x":"Stopped") + ")");
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
