#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* datamodel.h
 * Project: CPP
 * Description: MainWindow class - whole GUI
 * (mainwindow.h + mainwindow.cpp + mainwindowzoom.cpp + uilogic.cpp)
 *
 * Authors: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 *          Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QGraphicsEllipseItem>
#include <QMainWindow>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTreeView>

#include "datamodel.h"
#include "maptimer.h"

#define NONE_SELECTED (-1)

class StreetItem;
class TrafficCircleItem;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief HighlightStreetsInTrip Highlight all streets in currently selected trip
     * @param trip
     */
    void HighlightStreetsInTrip(const Trip * const trip);

public slots:
    void ListSelectionChanged(QModelIndex index);

private slots:
    void selectionChanged();
    void on_toggleTimeBtn_clicked();
    void on_fasterBtn_pressed();
    void on_fasterBtn_released();
    void on_slowerBtn_pressed();
    void on_slowerBtn_released();
    void on_normalBtn_clicked();
    void ZoomInBtn_clicked();
    void ZoomOutBtn_clicked();
    void vehicleSelectionChanged(const Trip * trip);
    void on_resetBtn_clicked();
    void TrafficSliderChanged(int value);
    void on_resettrafficBtn_clicked();

private:

    /**
     * @brief InitScene initialize scene with
     * @param data
     */
    void InitScene(DataModel* data);

    /**
    * @brief AddZoomButtons Add zoom buttons to the window
    */
    void AddZoomButtons();

    /**
     * @brief SceneZoomIn Zoom in map view
     */
    void SceneZoomIn();

    /**
     * @brief SceneZoomOut Zoom out map view
     */
    void SceneZoomOut();

    void initializeTimers();
    void updateTime();
    void incrementMultiplier();
    void startAutoIncrement();
    void multiplyMultiplicator();

    void redrawVehicles(QTime time);
    void invalidateVehicles();
    void freeInvalidVehicles();

    Ui::MainWindow *ui;

    std::vector<TrafficCircleItem*> drawnVehicles;
    MapTimer *mapTimer;
    double currentIncrement; // multiplier increment
    QTimer *incrementsModifierTimer; // when timeout is signalled, enlarges the multiplier increment value
    QTimer *incrementTimer; // automatically modifies time multiplicator with the user holding the button down
    QTimer *incrementWaiterTimer; // waits before starts automatically incrementing

    QStandardItemModel *treeViewModel;
    DataModel * data;
    QGraphicsScene * scene;
    std::vector<StreetItem*> scene_streets;
    std::vector<QGraphicsEllipseItem*> scene_stops;
    QPushButton* btn_zoom_in;
    QPushButton* btn_zoom_out;

    std::vector<StreetItem*> selected_streets;
    int selected_street;


    // zoom range is measured in steps
    static constexpr int zoom_min = 1;
    static constexpr int zoom_max = 5;
    static int zoom_current; // = 1 (on-init)

    // size of a zoom step
    static constexpr qreal zoom_scale_factor = 5.0/4;
};

#endif // MAINWINDOW_H
