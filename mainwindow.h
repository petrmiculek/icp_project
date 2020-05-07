#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datamodel.h"
#include "maptimer.h"

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QMainWindow>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QLabel>

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

    void on_slowerBtn_clicked();

    void on_fasterBtn_clicked();

    void on_lotfasterBtn_clicked();

    void on_lotslowerBtn_clicked();

    void ZoomInBtn_clicked();

    void ZoomOutBtn_clicked();

private:
    Ui::MainWindow *ui;

    MapTimer *mapTimer;
    QLabel *time_label;
    QLabel *status_label;
    void updateTime();
    void incrementMultiplier(double increment);

    std::vector<QGraphicsLineItem*> scene_streets;
    std::vector<QGraphicsEllipseItem*> scene_stops;

    static const constexpr qreal zoom_scale_factor = 5.0/4;
};

#endif // MAINWINDOW_H
