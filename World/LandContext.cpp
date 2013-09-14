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

#include "LandContext.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{
LandContext::LandContext(uint64_t id): InteractionContext(id)
{
}

uint32_t LandContext::type() const
{
    return WorldHelper::TYPE_LAND_CONTEXT;
}

bool LandContext::deserialize(std::istream& stream)
{
    return InteractionContext::deserialize(stream);
}

bool LandContext::serialize(std::ostream& stream) const
{
    return InteractionContext::serialize(stream);
}
}
}
