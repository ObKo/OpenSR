#include "EditorView.h"
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include "PatchItem.h"

namespace QNPE
{
EditorView::EditorView(QWidget *parent) :
    QGraphicsView(parent)
{
    setBackgroundBrush(QBrush(Qt::lightGray));
    m_zoom = 1.0f;
}

void EditorView::wheelEvent(QWheelEvent *event)
{
    int numSteps = event->delta() / 120;
    qreal factor = 1.0 + qreal(numSteps) / 20.0;
    setZoom(m_zoom * factor);
}

void EditorView::setZoom(qreal zoom)
{
    if(zoom <= 0.0)
        return;

    scale(zoom / m_zoom, zoom / m_zoom);
    m_zoom = zoom;
}

qreal EditorView::zoom() const
{
    return m_zoom;
}

void EditorView::contextMenuEvent(QContextMenuEvent * event)
{
    QGraphicsItem *i = scene()->itemAt(mapToScene(event->pos()));
    PatchItem *item = dynamic_cast<PatchItem*>(i);

    if(!item)
        return;

    QPointF itemPoint = item->mapFromScene(mapToScene(event->pos()));
    QPoint removeCell = item->mapToGrid(itemPoint);

    QMenu menu;

    QAction *removeRowAction = menu.addAction(tr("Remove row"));
    QAction *removeColumnAction = menu.addAction(tr("Remove column"));
    QAction *addRowAction = menu.addAction(tr("Add row"));
    QAction *addColumnAction = menu.addAction(tr("Add column"));
    QAction *normalizeAction = menu.addAction(tr("Normalize"));

    QAction *selectedAction = menu.exec(mapToGlobal(event->pos()));

    if(selectedAction == removeRowAction)
    {
        item->removeRow(removeCell.y());
    }
    else if(selectedAction == removeColumnAction)
    {
        item->removeColumn(removeCell.x());
    }
    else if(selectedAction == addColumnAction)
    {
        item->addColumn(itemPoint);
    }
    else if(selectedAction == addRowAction)
    {
        item->addRow(itemPoint);
    }
    else if(selectedAction == normalizeAction)
    {
        item->normalize();
    }
}

}
