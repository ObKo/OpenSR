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

#include <boost/shared_ptr.hpp>

namespace Rangers
{
namespace World
{
class RANGERS_WORLD_API HabitablePlanet: public Planet
{
public:
    HabitablePlanet(uint64_t id = 0);

    uint32_t invader() const;
    uint32_t population() const;
    boost::shared_ptr<LandContext> landContext() const;

    void setPopulation(uint32_t population);
    void setInvader(uint32_t invader);
    void setLandContext(boost::shared_ptr<LandContext> landContext);

    virtual uint32_t type() const;
    virtual bool serialize(std::ostream &stream) const;
    virtual bool deserialize(std::istream &stream);
    virtual std::list<uint64_t> dependencies();

protected:
    uint32_t m_population;
    uint32_t m_invader;
    boost::shared_ptr<LandContext> m_landContext;
};
}
}

#endif