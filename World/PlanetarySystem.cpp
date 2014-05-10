/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2013 Kosyak <ObKo@mail.ru>

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

#include "PlanetarySystem.h"

#include "WorldHelper.h"
#include "WorldManager.h"

#include <OpenSR/libRanger.h>

namespace Rangers
{
namespace World
{
PlanetarySystem::PlanetarySystem(uint64_t id) : WorldObject(id)
{
    m_position.x = 0.0f;
    m_position.y = 0.0f;
    m_size = 0.0f;
    m_background = 0;
    m_style = 0;
}

bool PlanetarySystem::deserialize(std::istream& stream)
{
    if (!WorldObject::deserialize(stream))
        return false;

    uint32_t nameLength;
    uint32_t objectCount;

    stream.read((char *)&m_position, sizeof(Point));
    if (!WorldHelper::deserializeString(m_name, stream))
        return false;

    stream.read((char *)&m_size, sizeof(float));
    stream.read((char *)&m_style, sizeof(uint32_t));
    stream.read((char *)&m_background, sizeof(uint32_t));

    stream.read((char *)&objectCount, sizeof(uint32_t));
    if (!stream.good())
        return false;

    for (int i = 0; i < objectCount; i++)
    {
        uint64_t id;
        stream.read((char *)&id, sizeof(uint64_t));

        boost::shared_ptr<SystemObject> object = boost::static_pointer_cast<SystemObject>(WorldManager::instance().getObject(id));
        addObject(object);

        if (!object)
            return false;

        if (!stream.good())
            return false;
    }

    return true;
}

std::string PlanetarySystem::name() const
{
    return m_name;
}

Point PlanetarySystem::position() const
{
    return m_position;
}

bool PlanetarySystem::serialize(std::ostream& stream) const
{
    if (!WorldObject::serialize(stream))
        return false;

    uint32_t nameLength = m_name.length();
    uint32_t objectCount = m_systemObjects.size();

    stream.write((const char *)&m_position, sizeof(Point));

    if (!WorldHelper::serializeString(m_name, stream))
        return false;

    stream.write((const char *)&m_size, sizeof(float));
    stream.write((const char *)&m_style, sizeof(uint32_t));
    stream.write((const char *)&m_background, sizeof(uint32_t));

    stream.write((const char *)&objectCount, sizeof(uint32_t));

    std::list<boost::shared_ptr<SystemObject> >::const_iterator end = m_systemObjects.end();
    for (std::list<boost::shared_ptr<SystemObject> >::const_iterator i = m_systemObjects.begin(); i != end; ++i)
    {
        uint64_t id = (*i)->id();
        stream.write((const char *)&id, sizeof(uint64_t));
    }

    if (!stream.good())
        return false;

    return true;
}

float PlanetarySystem::size() const
{
    return m_size;
}

std::list< boost::shared_ptr<SystemObject> > PlanetarySystem::systemObjects() const
{
    return m_systemObjects;
}

uint32_t PlanetarySystem::type() const
{
    return WorldHelper::TYPE_PLANETARYSYSTEM;
}

std::list< uint64_t > PlanetarySystem::dependencies()
{
    std::list<uint64_t> objectlist = WorldObject::dependencies();
    std::list<boost::shared_ptr<SystemObject> >::const_iterator end = m_systemObjects.end();
    for (std::list< boost::shared_ptr<SystemObject> >::const_iterator i = m_systemObjects.begin(); i != end; ++i)
    {
        objectlist.push_back((*i)->id());
    }
    return objectlist;
}

void PlanetarySystem::addObject(boost::shared_ptr<SystemObject> object)
{
    m_systemObjects.push_back(object);
}

void PlanetarySystem::removeObject(boost::shared_ptr<SystemObject> object)
{
    m_systemObjects.remove(object);
}

void PlanetarySystem::setName(const std::string& name)
{
    m_name = name;
}

void PlanetarySystem::setPosition(const Point& point)
{
    m_position = point;
}

void PlanetarySystem::setSize(float size)
{
    m_size = size;
}

void PlanetarySystem::turn(float progress)
{
    std::list<boost::shared_ptr<SystemObject> >::const_iterator end = m_systemObjects.end();
    for (std::list< boost::shared_ptr<SystemObject> >::const_iterator i = m_systemObjects.begin(); i != end; ++i)
    {
        (*i)->turn(progress);
    }
}

void PlanetarySystem::setStyle(uint32_t style)
{
    m_style = style;
}

void PlanetarySystem::setStyle(const std::string& style)
{
    m_style = textHash32(style);
}

uint32_t PlanetarySystem::style() const
{
    return m_style;
}

uint32_t PlanetarySystem::background() const
{
    return m_background;
}

void PlanetarySystem::setBackground(const std::string& bg)
{
    m_background = textHash32(bg);
}

void PlanetarySystem::setBackground(uint32_t bg)
{
    m_background = bg;
}

}
}