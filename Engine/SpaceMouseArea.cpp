/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OpenSR/SpaceMouseArea.h"

#include <QApplication>
#include <QStyleHints>

namespace OpenSR
{
SpaceMouseArea::SpaceMouseArea(QQuickItem *parent): QQuickItem(parent),
    m_pressed(false),
    m_containsMouse(false)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

void SpaceMouseArea::setPressed(bool pressed)
{
    if (m_pressed != pressed)
    {
        m_pressed = pressed;
        emit pressedChanged();
    }
}

void SpaceMouseArea::setContainsMouse(bool containsMouse)
{
    if (m_containsMouse != containsMouse)
    {
        m_containsMouse = containsMouse;
        emit containsMouseChanged();
    }
}

bool SpaceMouseArea::contains(const QPointF &point) const
{
    if (!QQuickItem::contains(point))
        return false;

    qreal x = point.x() - width() / 2.0f;
    qreal y = point.y() - height() / 2.0f;

    return (x * x + y * y) < (m_radius * m_radius);
}

void SpaceMouseArea::mousePressEvent(QMouseEvent *event)
{
    setPressed(true);
    m_pressPoint = event->pos();
    emit pressed();
}

void SpaceMouseArea::mouseReleaseEvent(QMouseEvent *event)
{
    setPressed(false);
    emit released();

    const int threshold = qobject_cast<QApplication*>(qApp)->styleHints()->startDragDistance();
    const bool isClick = (threshold >= qAbs(event->x() - m_pressPoint.x()) &&
                          threshold >= qAbs(event->y() - m_pressPoint.y()));

    if (isClick)
        emit clicked();
}

void SpaceMouseArea::mouseUngrabEvent()
{
    setPressed(false);
    emit canceled();
}

void SpaceMouseArea::hoverEnterEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    setContainsMouse(true);
    emit(entered());
}

void SpaceMouseArea::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event);
    setContainsMouse(false);
    emit(exited());
}

bool SpaceMouseArea::isPressed() const
{
    return m_pressed;
}

bool SpaceMouseArea::containsMouse() const
{
    return m_containsMouse;
}

qreal SpaceMouseArea::radius() const
{
    return m_radius;
}

void SpaceMouseArea::setRadius(qreal radius)
{
    if (radius != m_radius)
    {
        m_radius = radius;
        emit(radiusChanged());
    }
}
}
