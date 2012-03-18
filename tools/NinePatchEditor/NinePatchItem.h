#ifndef QNPE_NINEPATCHITEM_H
#define QNPE_NINEPATCHITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QVector>

namespace QNPE
{
class NinePatchItem : public QGraphicsItem
{
public:
    enum ResizeType {NONE, LEFT, RIGHT, TOP, BOTTOM, TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT};
    explicit NinePatchItem(const QPixmap& pixmap, const QRectF& geometry, QGraphicsItem *parent = 0);

    void reset(const QPixmap& pixmap, const QRectF& geometry,
               const QVector<qreal>& rows = QVector<qreal>(), const QVector<qreal>& columns = QVector<qreal>());
    void updateRects(const QVector<qreal>& rows, const QVector<qreal>& columns);

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual QRectF boundingRect() const;

protected:
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

private:
    QPixmap m_pixmap;
    QRectF m_geometry;

    ResizeType m_resize;

    QVector<QRectF> m_sources;
    QVector<QRectF> m_targets;

    QVector<qreal> m_rows;
    QVector<qreal> m_columns;
};
}

#endif // QNPE_NINEPATCHITEM_H
