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


#ifndef WORLD_PLANETMANAGER_H
#define WORLD_PLANETMANAGER_H

#include <map>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace Rangers
{
class Texture;
namespace World
{
struct PlanetStyle;
class PlanetManager
{
public:
    static PlanetManager& instance();

    boost::shared_ptr<PlanetStyle> style(const std::wstring& name);
    boost::shared_ptr<PlanetStyle> style(uint32_t id);

    void loadStyles(const std::wstring& styleFile);

    //! Get static planet image from planet style
    boost::shared_ptr<Texture> getPlanetImage(boost::shared_ptr<PlanetStyle> style, int size);
    //! Get static planet image from planet style id
    boost::shared_ptr<Texture> getPlanetImage(uint32_t style, int size);

    //! Drop planet image cache
    void dropImageCache();

private:
    PlanetManager();
    PlanetManager(const PlanetManager& other);

    std::map<uint32_t, boost::shared_ptr<PlanetStyle> > m_styles;
    std::map<uint64_t, boost::shared_ptr<Texture> > m_imageCache;
};
}
}

#endif // WORLD_PLANETMANAGER_H
