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
 */

#include <QTreeView>
#include <QMouseEvent>
#include <QDebug>

#include "mainwindow.h"


class DeselectableTreeView : public QTreeView
{
public:
    DeselectableTreeView(MainWindow * _window, QWidget *parent) : QTreeView(parent), window(_window) {}
    virtual ~DeselectableTreeView() {}

private:
    MainWindow * window;

    virtual void mousePressEvent(QMouseEvent *event)
    {
        QModelIndex item = indexAt(event->pos());
        bool selected = selectionModel()->isSelected(indexAt(event->pos()));
        QTreeView::mousePressEvent(event);

        if ((item.row() == -1 && item.column() == -1) || selected)
        {
            clearSelection();
            const QModelIndex index;

            auto prev = selectionModel()->selectedIndexes();

            selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
            //selectionModel()->select(item, QItemSelectionModel::Deselect);
        }
        window->selectionChanged();
    }
};
#endif // DESELECTABLETREEVIEW_H
