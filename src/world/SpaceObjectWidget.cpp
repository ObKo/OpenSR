/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#include "SpaceObjectWidget.h"
#include "SpaceObject.h"

#include <OpenSR/Sprite.h>

namespace Rangers
{
namespace World
{
SpaceObjectWidget::SpaceObjectWidget(boost::shared_ptr< SpaceObject > object): Widget()
{
    setObject(object);
}

SpaceObjectWidget::~SpaceObjectWidget()
{

}

void SpaceObjectWidget::draw() const
{
    if (!prepareDraw())
        return;

    if (m_sprite)
        m_sprite->draw();

    endDraw();
}

boost::shared_ptr<SpaceObject> SpaceObjectWidget::object() const
{
    return m_object;
}

void SpaceObjectWidget::processLogic(int dt)
{
    if (m_sprite)
        m_sprite->processLogic(dt);

    updatePosition();
}

void SpaceObjectWidget::setObject(boost::shared_ptr< SpaceObject > object)
{
    m_object = object;
    if (!m_object)
        return;
}

void SpaceObjectWidget::updatePosition()
{
    if (!m_object)
        return;
    Point p = m_object->position();
    setPosition(p.x, p.y);
}
}
}