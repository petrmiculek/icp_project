constexpr double BASEINCREMENT = 0.1;
constexpr int MAXMULTIPLIER =  10000; // maximum multiplier value (absolute value)

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringRef>

#include <math.h>

void MainWindow::multiplyMultiplicator()
{
    currentIncrement += currentIncrement*100;
    if (fabs(currentIncrement) > MAXMULTIPLIER/100) {
        currentIncrement = currentIncrement < 0 ? -MAXMULTIPLIER/100 : MAXMULTIPLIER/100;
    }
}

void MainWindow::initializeTimers()
{
    incrementsModifierTimer = new QTimer(this);
    incrementsModifierTimer->setInterval(2500);
    QObject::connect(incrementsModifierTimer, &QTimer::timeout, this, &MainWindow::multiplyMultiplicator);

    incrementTimer = new QTimer(this);
    incrementTimer->setInterval(100);
    QObject::connect(incrementTimer, &QTimer::timeout, this, &MainWindow::incrementMultiplier);

    incrementWaiterTimer = new QTimer(this);
    incrementWaiterTimer->setInterval(750);
    incrementWaiterTimer->setSingleShot(true);
    QObject::connect(incrementWaiterTimer, &QTimer::timeout, this, &MainWindow::startAutoIncrement);
}

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
    size_t mltplr_length = std::to_string(int(truncl(multiplier))).length() + 2;
    if (truncl(multiplier) == 0 && multiplier < 0)
        mltplr_length += 1;
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

void MainWindow::incrementMultiplier()
{
    double currentMltplr = mapTimer->getMultiplier() + currentIncrement;

    if (fabs(currentMltplr) > MAXMULTIPLIER) {
        currentMltplr = currentMltplr < 0 ? -MAXMULTIPLIER : MAXMULTIPLIER;
    }

    mapTimer->setMultiplier(currentMltplr);
}

void MainWindow::startAutoIncrement()
{ incrementTimer->start(); incrementsModifierTimer->start(); }

void MainWindow::on_fasterBtn_pressed()
{
    currentIncrement = BASEINCREMENT;
    incrementMultiplier();
    incrementWaiterTimer->start();
}

void MainWindow::on_fasterBtn_released()
{
    incrementWaiterTimer->stop();
    incrementTimer->stop();
    incrementsModifierTimer->stop();
}

void MainWindow::on_slowerBtn_pressed()
{
    currentIncrement = -BASEINCREMENT;
    incrementMultiplier();
    incrementWaiterTimer->start();
}

void MainWindow::on_slowerBtn_released()
{
    incrementWaiterTimer->stop();
    incrementTimer->stop();
    incrementsModifierTimer->stop();
}

void MainWindow::on_normalBtn_clicked()
{
    mapTimer->setMultiplier(1);
}

void MainWindow::on_resetBtn_clicked()
{
    mapTimer->reset();
}
