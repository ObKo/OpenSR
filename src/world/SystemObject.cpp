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

#include "SystemObject.h"

namespace Rangers
{
namespace World
{
bool SystemObject::deserialize(std::istream& stream)
{
    return SpaceObject::deserialize(stream);
}

std::wstring SystemObject::name() const
{
    return m_name;
}

bool SystemObject::serialize(std::ostream& stream) const
{
    return SpaceObject::serialize(stream);
}

uint32_t SystemObject::type() const
{
    return 0x127dbfab;
}

}
}
