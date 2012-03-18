#ifndef QNPE_BACKGROUNDITEM_H
#define QNPE_BACKGROUNDITEM_H

#include <QGraphicsItem>
#include <QBrush>

namespace QNPE
{
class BackgroundItem : public QGraphicsRectItem
{
public:
    explicit BackgroundItem(const QRectF& rect, QGraphicsItem *parent = 0);

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

private:
    QBrush m_brush;
};
}

#endif // QNPE_BACKGROUNDITEM_H
