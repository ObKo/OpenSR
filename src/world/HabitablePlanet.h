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

#ifndef WORLD_HABITABLE_PLANET_H
#define WORLD_HABITABLE_PLANET_H

#include "Planet.h"
#include "LandContext.h"

namespace Rangers
{
namespace World
{
class HabitablePlanet: public Planet
{
public:
    HabitablePlanet(uint64_t id = 0);

    uint32_t invader() const;
    uint32_t population() const;
    uint32_t race() const;
    LandContext landContext() const;

    void setPopulation(uint32_t population);
    void setInvader(uint32_t invader);
    void setRace(uint32_t race);
    void setLandContext(const LandContext& landContext);

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);
protected:
    uint32_t m_population;
    uint32_t m_invader;
    uint32_t m_race;
    LandContext m_landContext;
};
}
}

#endif