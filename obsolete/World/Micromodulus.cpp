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

#include "Micromodulus.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{
Micromodulus::Micromodulus(uint64_t id): Item(id)
{
}

bool Micromodulus::deserialize(std::istream& stream)
{
    return Item::deserialize(stream);
}

bool Micromodulus::serialize(std::ostream& stream) const
{
    return Item::serialize(stream);
}

uint32_t Micromodulus::type() const
{
    return WorldHelper::TYPE_MICROMODULUS;
}
}
}
