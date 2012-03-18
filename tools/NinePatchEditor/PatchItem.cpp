#include "PatchItem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QMenu>
#include "EditorView.h"

namespace
{
const QBrush PATCH_VERTHORIZ_BRUSH = QBrush(QColor(0, 192, 0, 64));
const QPen PATCH_VERTHORIZ_PEN = QPen(QColor(0, 96, 0));
const QBrush PATCH_BRUSH = QBrush(QColor(192, 0, 192, 64));
const QPen PATCH_PEN = QPen(Qt::transparent);
struct Delta
{
    Delta(qreal d1_, qreal d2_)
    {
        d1 = d1_;
        d2 = d2_;
    }

    Delta()
    {
        d1 = 0.0;
        d2 = 0.0;
    }

    qreal d1, d2;
};
}

namespace QNPE
{
PatchItem::PatchItem(const QRectF& imageRect, QGraphicsItem *parent): QGraphicsItem(parent), m_imageRect(imageRect)
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    m_currentIndex = -1;
    updateRects();
}

void PatchItem::reset(const QRectF& imageRect, const QVector<qreal>& rows, const QVector<qreal>& columns)
{
    m_currentIndex = -1;
    m_imageRect = imageRect;
    m_rows = rows;
    m_columns = columns;
    m_moveRow = false;
    updateRects();
}

void PatchItem::updateRects()
{
    m_vhRects.clear();
    m_centralRects.clear();

    bool sizeable = false;

    QVector<Delta> rows, columns;
    qreal prev = 0.0f;

    foreach(qreal row, m_rows)
    {
        if(sizeable)
            rows << Delta(prev, row);
        prev = row;
        sizeable = !sizeable;
    }
    if(sizeable)
        rows << Delta(prev, 1.0f);

    sizeable = false;
    prev = 0.0;

    foreach(qreal column, m_columns)
    {
        if(sizeable)
            columns << Delta(prev, column);
        prev = column;
        sizeable = !sizeable;
    }
    if(sizeable)
        columns << Delta(prev, 1.0f);

    prepareGeometryChange();
    foreach(Delta row, rows)
    {
        m_vhRects << QRectF(0, row.d1 * m_imageRect.height(), m_imageRect.width(), (row.d2 - row.d1) * m_imageRect.height());
    }

    foreach(Delta column, columns)
    {
        m_vhRects << QRectF(column.d1 * m_imageRect.width(), 0, (column.d2 - column.d1) * m_imageRect.width(), m_imageRect.height());
    }

    foreach(Delta row, rows)
    {
        foreach(Delta column, columns)
        {
            m_centralRects << QRectF(column.d1 * m_imageRect.width(), row.d1 * m_imageRect.height(),
                                     (column.d2 - column.d1) * m_imageRect.width(), (row.d2 - row.d1) * m_imageRect.height());
        }
    }
}

QVector<qreal> PatchItem::rows() const
{
    return m_rows;
}

QVector<qreal> PatchItem::columns() const
{
    return m_columns;
}

void PatchItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QPainterPath path;

    foreach(QRectF r, m_vhRects)
    {
        path.addRect(r);
    }
    painter->save();
    painter->setBrush(PATCH_VERTHORIZ_BRUSH);
    painter->setPen(PATCH_VERTHORIZ_PEN);
    painter->drawPath(path);
    painter->setBrush(PATCH_BRUSH);
    painter->setPen(PATCH_PEN);

    foreach(QRectF r, m_centralRects)
    {
        painter->drawRect(r);
    }

    painter->restore();
}

QRectF PatchItem::boundingRect() const
{
    return m_imageRect;
}

void PatchItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
    qreal x = event->pos().x() / m_imageRect.width();
    qreal y = event->pos().y() / m_imageRect.height();

    int minRowIndex = -1;
    qreal minRow = 1.0f;

    int i = 0;

    foreach(qreal row, m_rows)
    {
        if(qAbs(row - y) < minRow)
        {
            minRow = qAbs(row - y);
            minRowIndex = i;
        }
        i++;
    }
    int minColumnIndex = -1;
    qreal minColumn = 1.0f;
    i = 0;
    foreach(qreal column, m_columns)
    {
        if(qAbs(column - x) < minColumn)
        {
            minColumn = qAbs(column - x);
            minColumnIndex = i;
        }
        i++;
    }

    if((minRowIndex < 0) && (minColumnIndex < 0))
        return;

    if(qMin(minRow, minColumn) > qMax(1.0f / m_imageRect.height(), 1.0f / m_imageRect.width()))
    {
        QCursor c = cursor();
        c.setShape(Qt::ArrowCursor);
        setCursor(c);
        return;
    }

    bool rows = minColumn > minRow;

    QCursor c = cursor();
    if(rows)
        c.setShape(Qt::SizeVerCursor);
    else
        c.setShape(Qt::SizeHorCursor);
    setCursor(c);
}

void PatchItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    m_currentIndex = -1;
}

void PatchItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    addRow(event->pos());
    addColumn(event->pos());
}


void PatchItem::normalize()
{
    //FIXME: looks strange.
    if(!m_rows.count())
        return;

    QVector<qreal>::iterator prev = m_rows.begin();
    QVector<qreal>::iterator end = m_rows.end();
    QList<int> removing;
    for(QVector<qreal>::iterator i = m_rows.begin() + 1; i != end; ++i)
    {
        if(*prev == *i)
        {
            if(!removing.contains(prev - m_rows.begin()))
                removing << prev - m_rows.begin();
            if(!removing.contains(i - m_rows.begin()))
                removing << i - m_rows.begin();
        }
        prev++;
    }
    if(((*(end - 1)) >= 1.0f) && (!removing.contains(end - 1 - m_rows.begin())))
        removing << end - 1 - m_rows.begin();
    qSort(removing);
    int d = 0;
    foreach(int i, removing)
    {
        if((d > i) || ((i - d) >= m_rows.count()))
            break;
        m_rows.remove(i - d);
        d++;
    }

    prev = m_columns.begin();
    end = m_columns.end();
    removing.clear();
    for(QVector<qreal>::iterator i = m_columns.begin() + 1; i != end; ++i)
    {
        if(*prev == *i)
        {
            if(!removing.contains(prev - m_columns.begin()))
                removing << prev - m_columns.begin();
            if(!removing.contains(i - m_columns.begin()))
                removing << i - m_columns.begin();
        }
        prev++;
    }
    if(((*(end - 1)) >= 1.0f) && (!removing.contains(end - 1 - m_columns.begin())))
        removing << end - 1 - m_columns.begin();
    qSort(removing);
    d = 0;
    foreach(int i, removing)
    {
        if((d > i) || ((i - d) >= m_columns.count()))
            break;
        m_columns.remove(i - d);
        d++;
    }

    updateRects();
}

void PatchItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if(m_currentIndex < 0)
        return;

    if(m_moveRow)
    {
        if(m_currentIndex > m_rows.count() - 1)
            return;
        qreal y = event->pos().y() / m_imageRect.height();
        qreal min;
        qreal max;
        if(m_currentIndex == 0)
            min = 0.0f;
        else
            min = m_rows[m_currentIndex - 1];

        if(m_currentIndex == m_rows.count() - 1)
            max = 1.0f;
        else
            max = m_rows[m_currentIndex + 1];

        m_rows[m_currentIndex] = qMin(qMax(min, y), max);
    }
    else
    {
        if(m_currentIndex > m_columns.count() - 1)
            return;
        qreal x = event->pos().x() / m_imageRect.width();

        qreal min;
        qreal max;
        if(m_currentIndex == 0)
            min = 0.0f;
        else
            min = m_columns[m_currentIndex - 1];

        if(m_currentIndex == m_columns.count() - 1)
            max = 1.0f;
        else
            max = m_columns[m_currentIndex + 1];

        m_columns[m_currentIndex] = qMin(qMax(min, x), max);
    }
    updateRects();
}

void PatchItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qreal x = event->pos().x() / m_imageRect.width();
    qreal y = event->pos().y() / m_imageRect.height();

    int minRowIndex = -1;
    qreal minRow = 1.0f;

    int i = 0;

    foreach(qreal row, m_rows)
    {
        if(qAbs(row - y) < minRow)
        {
            minRow = qAbs(row - y);
            minRowIndex = i;
        }
        i++;
    }
    int minColumnIndex = -1;
    qreal minColumn = 1.0f;
    i = 0;
    foreach(qreal column, m_columns)
    {
        if(qAbs(column - x) < minColumn)
        {
            minColumn = qAbs(column - x);
            minColumnIndex = i;
        }
        i++;
    }

    if((minRowIndex < 0) && (minColumnIndex < 0))
        return;

    if(qMin(minRow, minColumn) > qMax(1.0f / m_imageRect.height(), 1.0f / m_imageRect.width()))
    {
        QCursor c = cursor();
        c.setShape(Qt::ArrowCursor);
        setCursor(c);
        return;
    }

    bool rows = minColumn > minRow;

    QCursor c = cursor();
    if(rows)
        c.setShape(Qt::SizeVerCursor);
    else
        c.setShape(Qt::SizeHorCursor);
    setCursor(c);

    m_moveRow = rows;
    m_currentIndex = rows ? minRowIndex : minColumnIndex;
}

QPoint PatchItem::mapToGrid(const QPointF &point) const
{
    qreal x = point.x() / m_imageRect.width();
    qreal y = point.y() / m_imageRect.height();
    int row = qLowerBound(m_rows, y) - m_rows.begin();
    int column = qLowerBound(m_columns, x) - m_columns.begin();
    return QPoint(column, row);
}

void PatchItem::removeRow(int row)
{
    if((row < 0) || (row > m_rows.count()))
        return;

    if(!m_rows.count())
        return;

    if(row == 0)
    {
        m_rows[0] = 0.0;
    }
    else if(row == m_rows.count())
    {
        m_rows.pop_back();
    }
    else
    {
        m_rows.remove(row - 1, 2);
    }
    updateRects();
}

void PatchItem::removeColumn(int column)
{
    if((column < 0) || (column > m_columns.count()))
        return;

    if(!m_columns.count())
        return;

    if(column == 0)
    {
        m_columns[0] = 0.0;
    }
    else if(column == m_columns.count())
    {
        m_columns.pop_back();
    }
    else
    {
        m_columns.remove(column - 1, 2);
    }
    updateRects();
}

void PatchItem::addRow(const QPointF &point)
{
    qreal start = point.y() / m_imageRect.height();

    if((start > 1.0) || (start < 0.0))
        return;

    m_rows << start << start + 1.0f / m_imageRect.height();
    qSort(m_rows);
    updateRects();
}

void PatchItem::addColumn(const QPointF &point)
{
    qreal start = point.x() / m_imageRect.width();

    if((start > 1.0) || (start < 0.0))
        return;

    m_columns << start << start + 1.0f / m_imageRect.width();
    qSort(m_columns);
    updateRects();
}

void PatchItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    QCursor c = cursor();
    c.setShape(Qt::ArrowCursor);
    setCursor(c);
}

}
