/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_PLANETARY_SYSTEM_H
#define WORLD_PLANETARY_SYSTEM_H

#include "WorldObject.h"
#include "SystemObject.h"
#include <list>
#include <boost/shared_ptr.hpp>

namespace Rangers
{
namespace World
{
class PlanetarySystem: public WorldObject
{
public:
    PlanetarySystem(uint64_t id = 0);

    std::string name() const;
    Point position() const;
    std::list< boost::shared_ptr<SystemObject> > systemObjects() const;
    float size() const;

    void addObject(boost::shared_ptr<SystemObject> object);
    void removeObject(boost::shared_ptr<SystemObject> object);

    void setName(const std::string& name);
    void setPosition(const Point& point);
    void setSize(float size);

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);
    virtual std::list<uint64_t> dependencies();
    virtual void turn(float progress);

protected:
    Point m_position;
    std::string m_name;
    std::list< boost::shared_ptr<SystemObject> > m_systemObjects;
    float m_size;
};
}
}

#endif