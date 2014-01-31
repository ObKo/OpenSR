/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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

#include "RaceManager.h"
#include "WorldHelper.h"

#include <OpenSR/libRanger.h>
#include <OpenSR/ResourceManager.h>
#include <OpenSR/Log.h>
#include <json/value.h>

#include <boost/algorithm/string.hpp>

namespace
{
const uint32_t RACE_MANAGER_SIGNATURE = *((uint32_t*)"SRRM");
const uint32_t RACE_SIGNATURE = *((uint32_t*)"SRRS");
}

namespace Rangers
{
namespace World
{
namespace
{
//! Dummy struct for representing information about races as ResourceObject
struct Races: ResourceObject
{
    std::map<std::string, boost::shared_ptr<Race> > races;
    std::vector<std::tuple<std::string, std::string, float> > relations;
    std::vector<std::string> invaders;
};

class RacesFactory: public ResourceObjectManager::ResourceObjectFactory
{
    virtual boost::shared_ptr< ResourceObject > operator()(const std::string& currentPath, const Json::Value& object, ResourceObjectManager& manager)
    {
        boost::shared_ptr<Races> result = boost::shared_ptr<Races>(new Races());

        Json::Value jsonRaces = object.get("races", Json::Value());

        for (const std::string& s : jsonRaces.getMemberNames())
        {
            Json::Value r = jsonRaces.get(s, Json::Value());

            if (!r.isObject())
                continue;

            boost::shared_ptr<Race> race = boost::shared_ptr<Race>(new Race());
            race->id = textHash32(s);
            race->icon = boost::dynamic_pointer_cast<TextureRegionDescriptor>(manager.loadObject(r.get("icon", Json::Value()), "texture"));
            race->name = r.get("name", "").asString();
            race->invader = false;
            race->relations[race->id] = 1.0f;

            result->races[s] = race;
        }

        for (const std::string& s : object.get("relations", Json::Value()).getMemberNames())
        {
            float r;
            std::vector<std::string> races;
            try
            {
                r = object.get(s, 0).asInt();
                boost::split(races, s, boost::is_any_of(":"));

                if (races.size() != 2)
                    throw;

                if ((result->races.find(races[0]) == result->races.end()) || (result->races.find(races[1]) == result->races.end()))
                    throw;
            }
            catch (...)
            {
                Log::error() << "Invalid race relation: " << s;
                continue;
            }

            result->relations.push_back(std::tuple<std::string, std::string, float>(races[0], races[1], r));
        }

        for (const Json::Value& v : object.get("invaders", Json::Value()))
        {
            result->invaders.push_back(v.asString());
        }

        return result;
    }
};

}

RaceManager::RaceManager()
{
    ResourceManager::instance().objectManager().addFactory("races", boost::shared_ptr<ResourceObjectManager::ResourceObjectFactory>(new RacesFactory()));
}

void RaceManager::loadRaces(const std::string& file)
{
    m_races.clear();

    boost::shared_ptr<Races> races = ResourceManager::instance().objectManager().getObject<Races>(file);

    if (!races)
        return;

    std::vector<uint32_t> ids;

    for (const std::pair<std::string, boost::shared_ptr<Race> > &p : races->races)
    {
        m_races[textHash32(p.first)] = p.second;
        ids.push_back(textHash32(p.first));
    }

    std::sort(ids.begin(), ids.end());

    for (const std::tuple<std::string, std::string, float> &t : races->relations)
    {
        uint32_t id1 = textHash32(std::get<0>(t));
        uint32_t id2 = textHash32(std::get<1>(t));

        if (!std::binary_search(ids.begin(), ids.end(), id1))
            continue;
        if (!std::binary_search(ids.begin(), ids.end(), id2))
            continue;

        m_races[id1]->relations[id2] = std::get<2>(t);
        m_races[id2]->relations[id1] = std::get<2>(t);
    }

    //Check relations integrity
    for (const std::pair<uint32_t, boost::shared_ptr<Race> > &r : m_races)
    {
        for (uint32_t id : ids)
        {
            if (r.second->relations.find(id) == r.second->relations.end())
            {
                Log::error() << "No relation found for races \"" << r.second->name << "\" and \"" << m_races[id]->name << "\", using 0.0f (neutral)";
                m_races[id]->relations[r.first] = 0.0f;
                r.second->relations[id] = 0.0f;
            }
        }
    }

    for (const std::string &r : races->invaders)
    {
        uint32_t id = textHash32(r);
        if (!std::binary_search(ids.begin(), ids.end(), id))
            continue;
        m_races[id]->invader = true;
    }

    /*
    //TODO: relations
    boost::shared_ptr<std::istream> json = ResourceManager::instance().getFileStream(file);

    if (!json)
        return;

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(*json, root))
    {
        Log::error() << "Error parsing races JSON: " << reader.getFormatedErrorMessages();
        return;
    }
    if (!root.isObject())
    {
        Log::error() << "Invalid races JSON";
        return;
    }
    Json::Value races = root.get("races", Json::Value());
    Json::Value invaders = root.get("invaders", Json::Value());
    Json::Value relations = root.get("relations", Json::Value());
    if (!races.isObject() || !invaders.isArray() || !relations.isObject())
    {
        Log::error() << "Invalid races JSON";
        return;
    }
    Json::Value::Members members = races.getMemberNames();
    Json::Value::Members::const_iterator end = members.end();
    for (Json::Value::Members::const_iterator i = members.begin(); i != end; ++i)
    {
        Json::Value race = races[*i];
        boost::shared_ptr<Race> r = boost::shared_ptr<Race>(new Race());
        bool error;
        r->icon = JSONHelper::parseTextureRegion(race.get("icon", Json::Value()), error);
        r->id = textHash32(*i);
        r->invader = false;
        r->name = race.get("name", Json::Value()).asString();

        if (error)
        {
            Log::error() << "Invalid races JSON";
            m_races.clear();
            return;
        }

        m_races[r->id] = r;
    }
    Json::Value::iterator vend = invaders.end();
    for (Json::Value::iterator i = invaders.begin(); i != vend; ++i)
    {
        boost::shared_ptr<Race> r = race(textHash32((*i).asString()));
        if (r)
            r->invader = true;
    }
    members = relations.getMemberNames();
    end = members.end();
    for (Json::Value::Members::const_iterator i = members.begin(); i != end; ++i)
    {
        std::vector<std::string> ids = split(*i, ':');
        if (ids.size() < 2)
        {
            Log::error() << "Invalid races JSON - invalid relation id";
            m_races.clear();
            return;
        }
        float relation = relations[*i].asDouble();
        boost::shared_ptr<Race> first = race(ids[0]);
        boost::shared_ptr<Race> second = race(ids[1]);
        if (!first || !second)
        {
            Log::error() << "Invalid races JSON - unknown race in relations";
            m_races.clear();
            return;
        }
        first->relations[second->id] = relation;
        second->relations[first->id] = relation;
    }*/
}

bool Race::deserialize(std::istream& stream)
{
    uint32_t sig;
    stream.read((char*)&sig, 4);

    if (sig != RACE_SIGNATURE)
        return false;

    stream.read((char*)&id, 4);

    if (!stream.good())
        return false;

    if (!WorldHelper::deserializeString(name, stream))
        return false;

    icon = boost::shared_ptr<TextureRegionDescriptor>(new TextureRegionDescriptor());
    if (!WorldHelper::deserializeTextureRegion(*icon, stream))
        return false;

    stream.read((char *)&invader, sizeof(bool));

    if (!stream.good())
        return false;

    uint32_t relationCount;
    stream.read((char *)&relationCount, 4);

    for (int i = 0; i < relationCount; i++)
    {
        uint32_t id;
        float relation;

        stream.read((char *)&id, 4);
        //FIXME: sizeof(float) === 4?
        stream.read((char *)&relation, 4);

        if (!stream.good())
            return false;

        relations[id] = relation;
    }
    return true;
}

bool Race::serialize(std::ostream& stream) const
{
    stream.write((const char*)&RACE_SIGNATURE, 4);
    stream.write((const char*)&id, 4);

    if (!stream.good())
        return false;

    if (!WorldHelper::serializeString(name, stream))
        return false;

    if (icon)
    {
        if (!WorldHelper::serializeTextureRegion(*icon, stream))
            return false;
    }
    else
    {
        if (!WorldHelper::serializeTextureRegion(TextureRegionDescriptor(), stream))
            return false;
    }

    stream.write((const char *)&invader, sizeof(bool));

    uint32_t relationCount = relations.size();
    stream.write((const char *)&relationCount, 4);

    if (!stream.good())
        return false;

    std::map<uint32_t, float>::const_iterator end = relations.end();
    for (std::map<uint32_t, float>::const_iterator i = relations.begin(); i != end; ++i)
    {
        uint32_t id = (*i).first;
        float relation = (*i).second;

        stream.write((const char *)&id, 4);
        //FIXME: sizeof(float) === 4?
        stream.write((const char *)&relation, 4);

        if (!stream.good())
            return false;
    }
    return true;
}

bool RaceManager::deserialize(std::istream& stream)
{
    uint32_t size, sig;

    stream.read((char*)&sig, 4);

    if (sig != RACE_MANAGER_SIGNATURE)
        return false;

    stream.read((char*)&size, 4);

    if (!stream.good())
        return false;

    for (int i = 0; i < size; i++)
    {
        boost::shared_ptr<Race> r = boost::shared_ptr<Race>(new Race());
        if (!r->deserialize(stream))
            return false;

        m_races[r->id] = r;
    }
    return true;
}

bool RaceManager::serialize(std::ostream& stream) const
{
    uint32_t size = m_races.size();
    stream.write((const char*)&RACE_MANAGER_SIGNATURE, 4);
    stream.write((const char*)&size, 4);

    if (!stream.good())
        return false;

    std::map<uint32_t, boost::shared_ptr<Race> >::const_iterator end = m_races.end();
    for (std::map<uint32_t, boost::shared_ptr<Race> >::const_iterator i = m_races.begin(); i != end; ++i)
    {
        if (!(*i).second->serialize(stream))
            return false;
    }
    return true;
}

boost::shared_ptr<Race> RaceManager::race(uint32_t id) const
{
    std::map<uint32_t, boost::shared_ptr<Race> >::const_iterator v = m_races.find(id);
    if (v != m_races.end())
        return (*v).second;
    return boost::shared_ptr<Race>();
}

boost::shared_ptr<Race> RaceManager::race(const std::string& id) const
{
    return race(textHash32(id));
}

}
}
