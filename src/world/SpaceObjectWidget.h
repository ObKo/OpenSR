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

#ifndef WORLD_SPACE_OBJECT_WIDGET_H
#define WORLD_SPACE_OBJECT_WIDGET_H

#include <OpenSR/Widget.h>

namespace Rangers
{
class Sprite;
namespace World
{
class SpaceObject;
class SpaceObjectWidget: public Widget
{
public:
    SpaceObjectWidget(boost::shared_ptr<SpaceObject> object = boost::shared_ptr<SpaceObject>());
    virtual ~SpaceObjectWidget();

    virtual void draw() const;

    virtual void processLogic(int dt);

    boost::shared_ptr<SpaceObject> object() const;
    void setObject(boost::shared_ptr<SpaceObject> object);

protected:
    void updatePosition();

    boost::shared_ptr<Sprite> m_sprite;
    boost::shared_ptr<SpaceObject> m_object;
};
}
}
#endif // WORLD_SPACE_OBJECT_WIDGET_H
