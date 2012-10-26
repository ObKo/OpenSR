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

#include "Ship.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{
Ship::Ship(uint64_t id): SpaceObject(id)
{
}

ShipContext Ship::context() const
{
    return m_context;
}

bool Ship::deserialize(std::istream& stream)
{
    if (!SpaceObject::deserialize(stream))
        return false;

    uint32_t nameLength;

    stream.read((char *)&nameLength, sizeof(uint32_t));

    if (!stream.good())
        return false;

    char *str = new char[nameLength + 1];
    stream.read(str, nameLength);
    str[nameLength] = '\0';

    m_name = std::string(str);
    delete[] str;

    if (!stream.good())
        return false;

    if (!m_context.deserialize(stream))
        return false;

    return true;
}

std::string Ship::name() const
{
    return m_name;
}

bool Ship::serialize(std::ostream& stream) const
{
    if (!SpaceObject::serialize(stream))
        return false;

    uint32_t nameLength = m_name.length();

    stream.write((const char *)&nameLength, sizeof(uint32_t));
    stream.write(m_name.c_str(), m_name.length());

    if (!stream.good())
        return false;

    if (!m_context.serialize(stream))
        return false;

    return true;
}

uint32_t Ship::type() const
{
    return WorldHelper::TYPE_SHIP;
}

void Ship::setName(const std::string& name)
{
    m_name = name;
}

void Ship::setContext(const ShipContext& context)
{
    m_context = context;
}
}
}
