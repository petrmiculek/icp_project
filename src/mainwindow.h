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
    /**
     * @brief ListSelectionChanged Handle change of selection on list of lines
     * @param index which item was selected
     */
    void ListSelectionChanged(QModelIndex index);

private slots:

    /**
     * @brief vehicleSelectionChanged Handle change to vehicle selection
     * @param trip trip of the selected vehicle
     */
    void VehicleSelectionChanged(const Trip * trip);

    /**
     * @brief TrafficSliderChanged Handle change of traffic density slider value
     * @param value new slider value
     */
    void TrafficSliderChanged(int value);

    /**
     * @brief SceneSelectionChanged Handle change of selection in the GraphicsScene
     */
    void SceneSelectionChanged();

    /**
     * @brief on_toggleTimeBtn_clicked Handle "Start/Stop time" button click
     */
    void on_toggleTimeBtn_clicked();

    /**
     * @brief on_fasterBtn_pressed Handle "Make time go faster" button press
     * @note using pressed+released instead of clicked <= holding the button keeps increasing the time speed
     */
    void on_fasterBtn_pressed();

    /**
     * @brief on_fasterBtn_released Handle "Make time go faster" button release
     *
     * @note using pressed+released instead of clicked <= holding the button keeps increasing the time speed
     */
    void on_fasterBtn_released();

    /**
     * @brief on_slowerBtn_pressed Handle "Make time go faster" button press
     * @note using pressed+released instead of clicked <= holding the button keeps decreasing the time speed
     */
    void on_slowerBtn_pressed();

    /**
     * @brief on_fasterBtn_released Handle "Make time go faster" button release
     *
     * @note using pressed+released instead of clicked <= holding the button keeps decreasing the time speed
     */
    void on_slowerBtn_released();

    /**
     * @brief on_normalBtn_clicked Handle "Make time go default speed" button click
     */
    void on_normalBtn_clicked();

    /**
     * @brief ZoomInBtn_clicked Handle Zoom-in button click
     */
    void ZoomInBtn_clicked();

    /**
     * @brief ZoomOutBtn_clicked Handle Zoom-out button click
     */
    void ZoomOutBtn_clicked();

    /**
     * @brief on_resetBtn_clicked Handle Reset button click
     * @note button resets time and removes vehicles
     */
    void on_resetBtn_clicked();

    /**
     * @brief on_resettrafficBtn_clicked Reset all traffic density to zero
     */
    void on_resettrafficBtn_clicked();

private:

    /**
     * @brief InitializeScene Initialize scene with map data
     * @param data map data
     */
    void InitializeScene(DataModel* map_data);

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

    /**
     * @brief CreateVehiclesInTrip Create new vehicles for
     * @param trip Trip for which vehicles will be created
     * @param time Time at which vehicles are being spawned (current time)
     */
    void CreateVehiclesInTrip(Trip& trip, QTime time);

    /**
     * @brief UpdateVehicles Update vehicle positions and spawn new
     * @param time
     */
    void UpdateVehicles(QTime time);

    /**
     * @brief InvalidateAllVehicles Make all vehicles invalid => ready to despawn
     */
    void InvalidateAllVehicles();

    /**
     * @brief FreeInvalidVehicles Remove invalid vehicles from the scene and correctly delete their objects
     */
    void RemoveInvalidVehicles();

    // GUI Elements
    Ui::MainWindow *ui;
    QPushButton* btn_zoom_in;
    QPushButton* btn_zoom_out;

    // Time-related elements
    MapTimer *mapTimer;
    double currentIncrement; // multiplier increment
    QTimer *incrementsModifierTimer; // when timeout is signalled, enlarges the multiplier increment value
    QTimer *incrementTimer; // automatically modifies time multiplicator with the user holding the button down
    QTimer *incrementWaiterTimer; // waits before starts automatically incrementing

    // Data for GUI elements
    DataModel * map_data;
    QStandardItemModel *treeViewModel;
    QGraphicsScene * scene; // all map-elements displayed (streets, stops, vehicles)

    // Elements displayed in the scene
    std::vector<StreetItem*> scene_streets; // all streets displayed
    std::vector<TrafficCircleItem*> drawnVehicles; // all vehicles displayed

    // temporary elements
    std::vector<StreetItem*> selected_streets; // all streets currently selected
    int vehicles_selected{0}; // count of currently selected vehicles

    // zoom range is measured in steps
    static constexpr int zoom_min = 1;
    static constexpr int zoom_max = 4;
    static int zoom_current; // = 1 (on-init)

    // size of a zoom step
    static constexpr qreal zoom_scale_factor = 5.0/4;
};

#endif // MAINWINDOW_H
