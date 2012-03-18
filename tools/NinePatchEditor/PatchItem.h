#ifndef QNPE_PATCHITEM_H
#define QNPE_PATCH_H

#include <QGraphicsItem>
#include <QBrush>

namespace QNPE
{
class PatchItem : public QGraphicsItem
{
public:
    explicit PatchItem(const QRectF& imageRect, QGraphicsItem *parent = 0);

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    virtual QRectF boundingRect() const;

    void reset(const QRectF& imageRect = QRectF(),
               const QVector<qreal>& rows = QVector<qreal>(),
               const QVector<qreal>& columns = QVector<qreal>());

    QPoint mapToGrid(const QPointF &point) const;
    QVector<qreal> rows() const;
    QVector<qreal> columns() const;

    void removeRow(int row);
    void removeColumn(int column);

    void addRow(const QPointF &point);
    void addColumn(const QPointF &point);

    void normalize();

protected:
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);

private:
    void updateRects();

    QRectF m_imageRect;

    int m_currentIndex;
    bool m_moveRow;

    QVector<qreal> m_rows;
    QVector<qreal> m_columns;

    QVector<QRectF> m_vhRects;
    QVector<QRectF> m_centralRects;
};
}

#endif // QNPE_PATCHITEM_H
