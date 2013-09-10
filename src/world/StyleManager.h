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


#ifndef WORLD_STYLEMANAGER_H
#define WORLD_STYLEMANAGER_H

#include <map>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace Rangers
{
class Texture;
namespace World
{
struct PlanetStyle;
struct SystemStyle;
struct AsteroidStyle;
class StyleManager
{
public:
    StyleManager();

    boost::shared_ptr<PlanetStyle> planetStyle(const std::string& name);
    boost::shared_ptr<PlanetStyle> planetStyle(uint32_t id);

    boost::shared_ptr<SystemStyle> systemStyle(const std::string& name);
    boost::shared_ptr<SystemStyle> systemStyle(uint32_t id);

    boost::shared_ptr<AsteroidStyle> asteroidStyle(const std::string& name);
    boost::shared_ptr<AsteroidStyle> asteroidStyle(uint32_t id);

    //! Load planet styles from JSON file
    void loadPlanetStyles(const std::wstring& styleFile);
    //! Load system styles from JSON file
    void loadSystemStyles(const std::wstring& styleFile);
    //! Load asteroid styles from JSON file
    void loadAsteroidStyles(const std::wstring& styleFile);


    bool serialize(std::ostream &stream) const;
    bool deserialize(std::istream &stream);

private:
    StyleManager(const StyleManager& other);

    std::map<uint32_t, boost::shared_ptr<SystemStyle> > m_systemStyles;
    std::map<uint32_t, boost::shared_ptr<PlanetStyle> > m_planetStyles;
    std::map<uint32_t, boost::shared_ptr<AsteroidStyle> > m_asteroidStyles;
};
}
}

#endif // WORLD_STYLEMANAGER_H
