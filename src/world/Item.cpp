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

#include "Item.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{

Item::Item(uint64_t id): WorldObject(id)
{
    m_size = 0;
    m_cost = 0;
}

uint32_t Item::cost() const
{
    return m_cost;
}

bool Item::deserialize(std::istream& stream)
{
    if (!WorldObject::deserialize(stream))
        return false;

    uint32_t nameLength;

    stream.read((char *)&m_size, sizeof(uint32_t));
    stream.read((char *)&m_cost, sizeof(uint32_t));
    stream.read((char *)&nameLength, sizeof(uint32_t));

    if (!stream.good())
        return false;

    char *str = new char[nameLength + 1];
    stream.read(str, nameLength);
    str[nameLength] = '\0';

    if (!stream.good())
        return false;

    return true;
}

std::string Item::name() const
{
    return m_name;
}

bool Item::serialize(std::ostream& stream) const
{
    if (!WorldObject::serialize(stream))
        return false;
    uint32_t nameLength = m_name.length();

    stream.write((const char *)&m_size, sizeof(uint32_t));
    stream.write((const char *)&m_cost, sizeof(uint32_t));
    stream.write((const char *)&nameLength, sizeof(uint32_t));
    stream.write(m_name.c_str(), m_name.length());

    if (!stream.good())
        return false;

    return true;
}

uint32_t Item::size() const
{
    return m_size;
}

uint32_t Item::type() const
{
    return WorldHelper::TYPE_ITEM;
}

void Item::setName(const std::string& name)
{
    m_name = name;
}

void Item::setSize(uint32_t size)
{
    m_size = size;
}

void Item::setCost(uint32_t cost)
{
    m_cost = cost;
}
}
}
