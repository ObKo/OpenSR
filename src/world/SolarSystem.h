/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_SOLAR_SYSTEM_H
#define WORLD_SOLAR_SYSTEM_H

#include "WorldObject.h"
#include "SystemObject.h"
#include <list>
#include <boost/shared_ptr.hpp>

namespace Rangers
{
namespace World
{
class SolarSystem: public WorldObject
{
public:
    std::wstring name() const;
    Point position() const;
    std::list< boost::shared_ptr<SystemObject> > systemObjects() const;
    float size() const;

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);
protected:
    Point m_position;
    std::wstring m_name;
    std::list< boost::shared_ptr<SystemObject> > m_systemObjects;
    float m_size;
};
}
}

#endif