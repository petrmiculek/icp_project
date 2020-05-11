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
#include <QTreeView>
#include <QMutex>

#define NONE_SELECTED (-1)

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

public slots:
    void ListSelectionChanged(QModelIndex index);

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
    void TrafficSliderChanged(int value);
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
    QLabel *strttraffic_label;
    QTreeView *transport_tree_view;
    QSlider *traffic_slider;
    double currentIncrement; // multiplier increment
    QTimer *incrementsModifierTimer; // when timeout is signalled, enlarges the multiplier increment value
    QTimer *incrementTimer; // automatically modifies time multiplicator with the user holding the button down
    QTimer *incrementWaiterTimer; // waits before starts automatically incrementing

    DataModel * data;
    QGraphicsScene * scene;
    std::vector<StreetItem*> scene_streets;
    std::vector<QGraphicsEllipseItem*> scene_stops;

    std::vector<Street> selected_streets;
    int selected_street;
    int selected_scene_street;
    bool selecting;

    static constexpr qreal zoom_scale_factor = 5.0/4;
};

#endif // MAINWINDOW_H
