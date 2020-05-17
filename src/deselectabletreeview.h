#ifndef DESELECTABLETREEVIEW_H
#define DESELECTABLETREEVIEW_H

/* deselectabletreeview.h
 * Project: CPP
 * Description: Treeview that supports de-selection
 * (header-only)
 *
 * source:
 * https://stackoverflow.com/a/10826787/10501005
 * original author: Yassir Ennazk
 *
 * modified by: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 *              Petr Mičulek, FIT <xmicul08@stud.fit.vutbr.cz>
 */

#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>

#include "mainwindow.h"


class DeselectableTreeView : public QTreeView
{
public:
    DeselectableTreeView(QWidget *parent) : QTreeView(parent) {}
    virtual ~DeselectableTreeView() {}

    MainWindow * window;

private:
    /**
     * @brief Overridden QTreeView::mousePressEvent.
     * @param event Event data.
     */
    virtual void mousePressEvent(QMouseEvent *event) override
    {
        QModelIndex item = indexAt(event->pos());
        const bool selected = selectionModel()->isSelected(indexAt(event->pos()));
        QTreeView::mousePressEvent(event);

        if ((item.row() == -1 && item.column() == -1) || selected) {
            clearSelection();
            selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Select);
        }
        window->ListSelectionChanged(QModelIndex()); // deselects trip (empty selection)
    }
};
#endif // DESELECTABLETREEVIEW_H
