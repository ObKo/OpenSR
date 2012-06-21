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

#ifndef WORLD_HABITABLE_PLANET
#define WORLD_HABITABLE_PLANET

#include "Planet.h"
#include "LandContext.h"

namespace Rangers
{
namespace World
{
class HabitablePlanet: public Planet
{
public:
    uint32_t invader() const;
    uint32_t population() const;
    LandContext landContext() const;

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);
protected:
    uint32_t m_population;
    uint32_t m_invader;
    LandContext m_landContext;
};
}
}

#endif