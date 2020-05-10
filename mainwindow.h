#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datamodel.h"
#include "maptimer.h"
#include "trafficcircleitem.h"

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QMainWindow>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QLabel>
#include <QTimer>

class StreetItem;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void InitScene(DataModel* data);

    void AddZoomButtons();
    void SceneZoomIn();
    void SceneZoomOut();
private slots:
    void on_toggleTimeBtn_clicked();
    void on_fasterBtn_pressed();
    void on_fasterBtn_released();
    void on_slowerBtn_pressed();
    void on_slowerBtn_released();
    void on_normalBtn_clicked();
    void ZoomInBtn_clicked();
    void ZoomOutBtn_clicked();
    void selectionChanged();
    void on_resetBtn_clicked();
    // void RouteCreateToggled();

private:
    void initializeTimers();
    void updateTime();
    void redrawVehicles(QTime time);
    void incrementMultiplier();
    void startAutoIncrement();
    void multiplyMultiplicator();
    void initTrips();
    void invalidateVehicles();
    void deleteDrawnVehicles();


    Ui::MainWindow *ui;

    std::vector<TrafficCircleItem*> drawnVehicles;
    MapTimer *mapTimer;
    QLabel *time_label;
    QLabel *status_label;
    double currentIncrement; // multiplier increment
    QTimer *incrementsModifierTimer; // when timeout is signalled, enlarges the multiplier increment value
    QTimer *incrementTimer; // automatically modifies time multiplicator with the user holding the button down
    QTimer *incrementWaiterTimer; // waits before starts automatically incrementing

    DataModel * data;
    QGraphicsScene * scene;
    std::vector<StreetItem*> scene_streets;
    std::vector<QGraphicsEllipseItem*> scene_stops;

    std::vector<Street> selected_streets;
    bool selecting;

    static const constexpr qreal zoom_scale_factor = 5.0/4;

};

#endif // MAINWINDOW_H
