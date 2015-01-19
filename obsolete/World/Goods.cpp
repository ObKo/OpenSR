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

#include "Goods.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{
Goods::Goods(uint64_t id): Item(id)
{
}

bool Goods::deserialize(std::istream& stream)
{
    if (!Item::deserialize(stream))
        return false;

    stream.read((char *)&m_quantity, sizeof(uint32_t));
    stream.read((char *)&m_price, sizeof(uint32_t));

    if (!stream.good())
        return false;

    return true;
}

uint32_t Goods::price() const
{
    return m_price;
}

uint32_t Goods::quantity() const
{
    return m_quantity;
}

bool Goods::serialize(std::ostream& stream) const
{
    if (!Item::serialize(stream))
        return false;

    stream.write((const char *)&m_quantity, sizeof(uint32_t));
    stream.write((const char *)&m_price, sizeof(uint32_t));

    if (!stream.good())
        return false;

    return true;
}

uint32_t Goods::type() const
{
    return WorldHelper::TYPE_GOODS;
}

void Goods::setPrice(uint32_t price)
{
    m_price = price;
}

void Goods::setQuantity(uint32_t quantity)
{
    m_quantity = quantity;
}
}
}
