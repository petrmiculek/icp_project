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
     * @brief Highlights all streets in currently selected trip.
     * @param trip Currently selected trip.
     */
    void HighlightStreetsInTrip(const Trip * const trip);

public slots:
    /**
     * @brief Handles change of selection on list of lines.
     * @param index Selected item.
     */
    void ListSelectionChanged(QModelIndex index);

private slots:

    /**
     * @brief Handles vehicle's selection.
     * @param trip Trip the selected Vehicle belongs to.
     */
    void VehicleSelectionChanged(const Trip * trip);

    /**
     * @brief Handles change of traffic density slider value.
     * @param value New slider value.
     */
    void TrafficSliderChanged(int value);

    /**
     * @brief Handles change of selection in the GraphicsScene.
     */
    void SceneSelectionChanged();

    /**
     * @brief Handles "Start/Stop time" button click.
     */
    void on_toggleTimeBtn_clicked();

    /**
     * @brief Handles click on "faster speed" button.
     * @remarks Increases map time speed.
     * @note If the user keeps holding down the button, the timer speed is going to decrease in increasingly bigger chunks.
     */
    void on_fasterBtn_pressed();

    /**
     * @brief Handles click on "faster speed" button.
     * @details Resets all relevant timers that implement auto-incrementing.
     */
    void on_fasterBtn_released();

    /**
     * @brief Handles click on "faster speed" button.
     * @remarks Decreases map time speed.
     * @note If the user keeps holding down the button, the timer speed is going to decrease in increasingly bigger chunks.
     */
    void on_slowerBtn_pressed();

    /**
     * @brief Handles click on "slower speed" button.
     * @details Resets all relevant timers that implement auto-incrementing.
     */
    void on_slowerBtn_released();

    /**
     * @brief Handles click on "normal speed" button.
     * @remarks Resets time speed.
     */
    void on_normalBtn_clicked();

    /**
     * @brief Handles Zoom-in button click.
     */
    void ZoomInBtn_clicked();

    /**
     * @brief Handles Zoom-out button click.
     */
    void ZoomOutBtn_clicked();

    /**
     * @brief Handles Reset button click.
     * @remarks Button resets time and removes vehicles.
     */
    void on_resetBtn_clicked();

    /**
     * @brief Resets all traffic density to zero.
     */
    void on_resettrafficBtn_clicked();

private:

    /**
     * @brief Initializes scene with map data.
     * @param data Map data.
     */
    void InitializeScene(DataModel* map_data);

    /**
    * @brief Adds zoom buttons to the window.
    */
    void AddZoomButtons();

    /**
     * @brief Zooms in the map view
     */
    void SceneZoomIn();

    /**
     * @brief Zooms out the map view.
     */
    void SceneZoomOut();

    void initializeTimers();
    void updateTime();
    void incrementMultiplier();
    void startAutoIncrement();
    void multiplyMultiplicator();

    /**
     * @brief Creates new vehicles
     * @param trip Trip for which vehicles will be created.
     * @param time Time at which vehicles are being spawned (current time).
     */
    void CreateVehiclesInTrip(Trip& trip, QTime time);

    /**
     * @brief Updates vehicles' positions and spawns new ones.
     * @param time Spawn vehicles up to this time.
     */
    void UpdateVehicles(QTime time);

    /**
     * @brief Marks all vehicles as invalid and releases them from memory.
     * @remarks Used mainly when the timer (and therefore the scene) is reset.
     */
    void InvalidateAllVehicles();

    /**
     * @brief Removes invalid vehicles from the graphics scene and frees them from memory.
     */
    void RemoveInvalidVehicles();

    // GUI Elements
    Ui::MainWindow *ui;
    QPushButton* btn_zoom_in;
    QPushButton* btn_zoom_out;

    // Time-related elements
    MapTimer *mapTimer;
    /**
      * @brief Map time multiplier.
      */
    double currentIncrement;
    /**
      * @brief When timeout is signalled, enlarges the value auto-incremented to the time multiplier.
      * @remarks Timer is stopped and reset when the user releases the time modifying button.
      * @see MainWindow:currentIncrement
      */
    QTimer *incrementsModifierTimer;
    /**
      * @brief Auto-increments the time multiplicator as long as the user holds a time modifying button down.
      * @remarks Timer is stopped and reset when the user releases the time modifying button.
      * @see MainWindow:currentIncrement
      */
    QTimer *incrementTimer;
    /**
      * @brief When timeout is signalled, starts auto-incrementing the time multiplier.
      * @remarks Timer is stopped and reset when the user releases the time modifying button.
      * @note This timer ensures that the auto-incrementing doesn't start straightaway, therefore is unintentional.
      * @see MainWindow:currentIncrement
      */
    QTimer *incrementWaiterTimer;

    // Data for GUI elements
    DataModel * map_data;
    /**
     * @brief Pointer to a TreeView's model.
     * @remarks We keep this pointer in order to be able to free the model from the memory later on.
     */
    QStandardItemModel *treeViewModel;
    /**
     * @brief Graphics scene for all the vehicles, streets and stops.
     */
    QGraphicsScene * scene; // all map-elements displayed (streets, stops, vehicles)

    // Elements displayed in the scene
    /**
     * @brief Drawn streets.
     */
    std::vector<StreetItem*> scene_streets; // all streets displayed
    /**
     * @brief Drawn vehicles.
     */
    std::vector<TrafficCircleItem*> drawnVehicles; // all vehicles displayed

    // temporary elements
    std::vector<StreetItem*> selected_streets{}; // all streets currently selected
    int vehicles_selected{0}; // count of currently selected vehicles

    // zoom range is measured in steps
    static constexpr int zoom_min = 1;
    static constexpr int zoom_max = 4;
    static int zoom_current; // = 1 (on-init)

    // size of a zoom step
    static constexpr qreal zoom_scale_factor = 5.0/4;
};

#endif // MAINWINDOW_H
