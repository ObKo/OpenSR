#include "NinePatchItem.h"
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <cfloat>

namespace
{
struct Delta
{
    Delta(qreal t1_, qreal t2_, qreal size_)
    {
        t1 = t1_;
        t2 = t2_;
        size = size_;
    }

    Delta()
    {
        t1 = 0.0;
        t2 = 0.0;
        size = 0.0;
    }

    qreal t1, t2;
    qreal size;
};
}

namespace QNPE
{
NinePatchItem::NinePatchItem(const QPixmap& pixmap, const QRectF& geometry, QGraphicsItem *parent):
    QGraphicsItem(parent), m_pixmap(pixmap), m_geometry(geometry), m_resize(NONE)
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void NinePatchItem::updateRects(const QVector<qreal>& rows, const QVector<qreal>& columns)
{
    m_rows = rows;
    m_columns = columns;
    prepareGeometryChange();
    m_targets.clear();
    m_sources.clear();
    QVector<Delta> widths;
    QVector<Delta> heights;
    QVector<int> sizeable;

    bool currentSizeable = false;
    float residue = m_geometry.width();
    int size = columns.size();

    for (int i = 0; i != size; i++)
    {
        if (i != 0)
        {
            if (currentSizeable)
                sizeable << i;
            else
                residue -= (columns[i] - columns[i - 1]) * m_pixmap.width();
            widths << Delta(columns[i - 1], columns[i], (columns[i] - columns[i - 1]) * m_pixmap.width());
        }
        else
        {
            //if (columns[0] != 0.0)
            //{
                residue -= columns[0] * m_pixmap.width();
                widths << Delta(0, columns[0], columns[0] * m_pixmap.width());
            //}
        }
        currentSizeable = !currentSizeable;
    }
    if (size)
    {
        if (currentSizeable)
            sizeable << size;
        else
            residue -= (1.0 - columns[size - 1]) * m_pixmap.width();
        widths << Delta(columns[size - 1], 1.0, (1.0 - columns[size - 1]) * m_pixmap.width());
    }
    else
    {
        widths << Delta(0.0, 1.0, m_geometry.width());
        residue = 0.0;
    }

    size = sizeable.size();
    for (int i = 0; i < size; i++)
    {
        widths[sizeable[i]].size = residue / size;
    }

    sizeable.clear();
    currentSizeable = false;
    residue = m_geometry.height();
    size = rows.size();

    for (int i = 0; i != size; i++)
    {
        if (i != 0)
        {

            if (currentSizeable)
                sizeable.push_back(i);
            else
                residue -= (rows[i] - rows[i - 1]) * m_pixmap.height();
            heights << Delta(rows[i - 1], rows[i], (rows[i] - rows[i - 1]) * m_pixmap.height());
        }
        else
        {
            //if (rows[0] != 0.0)
            //{
                residue -= rows[0]  * m_pixmap.height();
                heights << Delta(0.0, rows[0], rows[0] * m_pixmap.height());
            //}
        }
        currentSizeable = !currentSizeable;
    }
    if (size)
    {
        if (currentSizeable)
            sizeable << size;
        else
            residue -= (1.0 - rows[size - 1]) * m_pixmap.height();
        heights << Delta(rows[size - 1], 1.0, (1.0 - rows[size - 1]) * m_pixmap.height());
    }
    else
    {
        heights << Delta(0.0, 1.0, m_geometry.height());
        residue = 0.0f;
    }

    size = sizeable.size();
    for (int i = 0; i < size; i++)
    {
        heights[sizeable[i]].size = residue / size;
    }

    qreal x = m_geometry.left();
    qreal y = m_geometry.top();
    foreach(Delta height, heights)
    {
        x = m_geometry.left();
        foreach(Delta width, widths)
        {
            m_targets << QRectF(x, y, width.size, height.size);
            m_sources << QRectF(width.t1 * m_pixmap.width(), height.t1 * m_pixmap.height(),
                                (width.t2 - width.t1) * m_pixmap.width() + FLT_EPSILON, (height.t2 - height.t1) * m_pixmap.height() + FLT_EPSILON);
            x += width.size;
        }
        y += height.size;
    }
}

void NinePatchItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if(m_sources.count() != m_targets.count())
        return;

    painter->save();

    for(int i = 0; i < m_sources.count(); i++)
        painter->drawPixmap(m_targets[i], m_pixmap, m_sources[i]);

    painter->restore();
}

QRectF NinePatchItem::boundingRect() const
{
    return m_geometry.adjusted(-5, -5, 5, 5);
}

void NinePatchItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
    bool left = false, right = false, top = false, bottom = false;

    event->accept();
    if(qAbs(event->pos().x() - m_geometry.left()) < 5)
        left = true;
    if(qAbs(event->pos().x() - m_geometry.right()) < 5)
        right = true;
    if(qAbs(event->pos().y() - m_geometry.top()) < 5)
        top = true;
    if(qAbs(event->pos().y() - m_geometry.bottom()) < 5)
        bottom = true;

    QCursor c = cursor();

    c.setShape(Qt::ArrowCursor);

    if(left || right)
        c.setShape(Qt::SizeHorCursor);
    if(top || bottom)
        c.setShape(Qt::SizeVerCursor);

    if((left && top) || (right && bottom))
        c.setShape(Qt::SizeFDiagCursor);

    if((right && top) || (left && bottom))
        c.setShape(Qt::SizeBDiagCursor);

    setCursor(c);
}

void NinePatchItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    bool left = false, right = false, top = false, bottom = false;

    event->accept();
    if(qAbs(event->pos().x() - m_geometry.left()) < 5)
        left = true;
    if(qAbs(event->pos().x() - m_geometry.right()) < 5)
        right = true;
    if(qAbs(event->pos().y() - m_geometry.top()) < 5)
        top = true;
    if(qAbs(event->pos().y() - m_geometry.bottom()) < 5)
        bottom = true;

    if(left)
        m_resize = LEFT;
    if(right)
        m_resize = RIGHT;

    if(top)
        m_resize = TOP;
    if(bottom)
        m_resize = BOTTOM;

    if(left && top)
        m_resize = TOPLEFT;
    if(right && bottom)
        m_resize = BOTTOMRIGHT;

    if(right && top)
        m_resize = TOPRIGHT;
    if(left && bottom)
        m_resize = BOTTOMLEFT;
}

void NinePatchItem::reset(const QPixmap& pixmap, const QRectF& geometry, const QVector<qreal>& rows, const QVector<qreal>& columns)
{
    m_pixmap = pixmap;
    m_geometry = geometry;
    m_resize = NONE;
    updateRects(rows, columns);
}

void NinePatchItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    switch(m_resize)
    {
    case LEFT:
    case TOPLEFT:
    case BOTTOMLEFT:
        m_geometry.setLeft(event->pos().x());
        break;
    case RIGHT:
    case TOPRIGHT:
    case BOTTOMRIGHT:
        m_geometry.setRight(event->pos().x());
        break;
    }
    switch(m_resize)
    {
    case TOP:
    case TOPLEFT:
    case TOPRIGHT:
        m_geometry.setTop(event->pos().y());
        break;
    case BOTTOM:
    case BOTTOMLEFT:
    case BOTTOMRIGHT:
        m_geometry.setBottom(event->pos().y());
        break;
    }

    if(m_geometry.width() < m_pixmap.width())
        m_geometry.setWidth(m_pixmap.width());

    if(m_geometry.height() < m_pixmap.height())
        m_geometry.setHeight(m_pixmap.height());

    m_geometry.moveTo(0, 0);

    updateRects(m_rows, m_columns);
}

void NinePatchItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    event->accept();
    m_resize = NONE;
}

}
