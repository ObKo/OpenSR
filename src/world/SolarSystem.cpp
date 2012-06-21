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

#include "SolarSystem.h"

namespace Rangers
{
namespace World
{
bool SolarSystem::deserialize(std::istream& stream)
{
    return WorldObject::deserialize(stream);
}

std::wstring SolarSystem::name() const
{
    return m_name;
}

Point SolarSystem::position() const
{
    return m_position;
}

bool SolarSystem::serialize(std::ostream& stream) const
{
    return WorldObject::serialize(stream);
}

float SolarSystem::size() const
{
    return m_size;
}

std::list< boost::shared_ptr<SystemObject> > SolarSystem::systemObjects() const
{
    return m_systemObjects;
}

uint32_t SolarSystem::type() const
{
    return 0x1c832836;
}

}
}