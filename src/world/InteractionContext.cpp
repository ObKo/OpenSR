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

#include "InteractionContext.h"
#include "WorldHelper.h"

namespace Rangers
{
namespace World
{
InteractionContext::InteractionContext(uint64_t id): WorldObject(id), m_relation(1.0f), m_race(0)
{
}

bool InteractionContext::deserialize(std::istream& stream)
{
    if (!WorldObject::deserialize(stream))
        return false;

    stream.read((char *)&m_race, sizeof(uint32_t));
    stream.read((char *)&m_relation, sizeof(float));

    if (!stream.good())
        return false;

    return true;
}

uint32_t InteractionContext::race() const
{
    return m_race;
}

float InteractionContext::relation() const
{
    return m_relation;
}

uint32_t InteractionContext::type() const
{
    return WorldHelper::TYPE_INTERACTION_CONTEXT;
}

void InteractionContext::setRace(uint32_t race)
{
    m_race = race;
}

void InteractionContext::setRelation(float relation)
{
    m_relation = relation;
}

bool InteractionContext::serialize(std::ostream& stream) const
{
    if (!WorldObject::serialize(stream))
        return false;

    stream.write((const char *)&m_race, sizeof(uint32_t));
    stream.write((const char *)&m_relation, sizeof(float));

    if (!stream.good())
        return false;

    return true;
}
}
}
