#include "BackgroundItem.h"
#include <QPainter>
#include <QDebug>
#include <QTransform>
#include <cmath>

namespace QNPE
{
BackgroundItem::BackgroundItem(const QRectF& rect, QGraphicsItem *parent) :
    QGraphicsRectItem(rect, parent)
{
    m_brush.setColor(Qt::lightGray);
    m_brush.setStyle(Qt::Dense4Pattern);
}

void BackgroundItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    //FIXME: need scale getting
    QLineF l = painter->transform().inverted().map(QLineF(0.0, 0.0, 1.0, 0.0));
    qreal scale = sqrt(l.dx() * l.dx() + l.dy() * l.dy());
    m_brush.setTransform(QTransform::fromScale(scale * 8, scale * 8));

    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(rect());

    painter->setBrush(m_brush);
    painter->drawRect(rect());

    painter->restore();
}
}
