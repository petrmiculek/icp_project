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

private slots:
    void on_toggleTimeBtn_clicked();

private:
    Ui::MainWindow *ui;

    MapTimer *mapTimer;
    QLabel *ui_label;
    void updateTime();

    std::vector<QGraphicsLineItem*> scene_streets;
    std::vector<QGraphicsEllipseItem*> scene_stops;
};

#endif // MAINWINDOW_H
