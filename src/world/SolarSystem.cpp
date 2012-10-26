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

#include "SolarSystem.h"
#include "WorldHelper.h"
#include "WorldManager.h"

namespace Rangers
{
namespace World
{
SolarSystem::SolarSystem(uint64_t id) : WorldObject(id)
{
    m_position.x = 0.0f;
    m_position.y = 0.0f;
    m_size = 0.0f;
}

bool SolarSystem::deserialize(std::istream& stream)
{
    if (!WorldObject::deserialize(stream))
        return false;

    uint32_t nameLength;
    uint32_t objectCount;

    stream.read((char *)&m_position, sizeof(Point));
    stream.read((char *)&nameLength, sizeof(uint32_t));

    if (!stream.good())
        return false;

    char *str = new char[nameLength + 1];
    stream.read(str, nameLength);
    str[nameLength] = '\0';

    m_name = std::string(str);
    delete[] str;

    stream.read((char *)&m_size, sizeof(float));

    stream.read((char *)&objectCount, sizeof(uint32_t));
    if (!stream.good())
        return false;

    for (int i = 0; i < objectCount; i++)
    {
        uint64_t id;
        stream.read((char *)&id, sizeof(uint64_t));

        boost::shared_ptr<SystemObject> object = boost::static_pointer_cast<SystemObject>(WorldManager::instance().getObject(id));

        if (!object)
            return false;

        if (!stream.good())
            return false;
    }

    return true;
}

std::string SolarSystem::name() const
{
    return m_name;
}

Point SolarSystem::position() const
{
    return m_position;
}

bool SolarSystem::serialize(std::ostream& stream) const
{
    if (!WorldObject::serialize(stream))
        return false;

    uint32_t nameLength = m_name.length();
    uint32_t objectCount = m_systemObjects.size();

    stream.write((const char *)&m_position, sizeof(Point));
    stream.write((const char *)&nameLength, sizeof(uint32_t));
    stream.write(m_name.c_str(), m_name.length());
    stream.write((const char *)&m_size, sizeof(float));
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

float SolarSystem::size() const
{
    return m_size;
}

std::list< boost::shared_ptr<SystemObject> > SolarSystem::systemObjects() const
{
    return m_systemObjects;
}

uint32_t SolarSystem::type() const
{
    return WorldHelper::TYPE_SOLARSYSTEM;
}

std::list< uint64_t > SolarSystem::dependencies()
{
    std::list<uint64_t> objectlist = WorldObject::dependencies();
    std::list<boost::shared_ptr<SystemObject> >::const_iterator end = m_systemObjects.end();
    for (std::list< boost::shared_ptr<SystemObject> >::const_iterator i = m_systemObjects.begin(); i != end; ++i)
    {
        objectlist.push_back((*i)->id());
    }
    return objectlist;
}

void SolarSystem::addObject(boost::shared_ptr<SystemObject> object)
{
    m_systemObjects.push_back(object);
}

void SolarSystem::removeObject(boost::shared_ptr<SystemObject> object)
{
    m_systemObjects.remove(object);
}

void SolarSystem::setName(const std::string& name)
{
    m_name = name;
}

void SolarSystem::setPosition(const Point& point)
{
    m_position = point;
}

void SolarSystem::setSize(float size)
{
    m_size = size;
}
}
}