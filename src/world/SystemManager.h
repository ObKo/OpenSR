/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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


#ifndef WORLD_SYSTEM_MANAGER_H
#define WORLD_SYSTEM_MANAGER_H

#include <list>
#include <map>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace Rangers
{
class Texture;
namespace World
{
class PlanetarySystem;
struct SystemStyle;
class SystemManager
{
public:
    SystemManager();

    boost::shared_ptr<SystemStyle> style(const std::string& name);
    boost::shared_ptr<SystemStyle> style(uint32_t id);

    //! Load system styles from JSON file
    void loadStyles(const std::wstring& styleFile);

    void addSystem(boost::shared_ptr<PlanetarySystem> system);
    std::list<boost::shared_ptr<PlanetarySystem> > systems() const;
    boost::shared_ptr<PlanetarySystem> currentSystem() const;

    void setCurrentSystem(boost::shared_ptr<PlanetarySystem> system);

    bool serialize(std::ostream &stream) const;
    bool deserialize(std::istream &stream);

private:
    SystemManager(const SystemManager& other);

    std::map<uint32_t, boost::shared_ptr<SystemStyle> > m_styles;

    std::list<boost::shared_ptr<PlanetarySystem> > m_systems;
    boost::shared_ptr<PlanetarySystem> m_currentSystem;
};
}
}

#endif
