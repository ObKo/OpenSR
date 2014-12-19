/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 - 2014 Kosyak <ObKo@mail.ru>

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

#ifndef WORLD_WORLDHELPER_H
#define WORLD_WORLDHELPER_H

#include <stdint.h>

#include <string>
#include <iostream>
#include <boost/shared_ptr.hpp>

namespace Rangers
{
class TextureRegionDescriptor;
namespace World
{
class WorldObject;
class RANGERS_WORLD_API WorldHelper
{
public:
    static const uint32_t TYPE_ARTIFACT;
    static const uint32_t TYPE_CARGOHOOK;
    static const uint32_t TYPE_DEFENCEGENERATOR;
    static const uint32_t TYPE_DESERTPLANET;
    static const uint32_t TYPE_DROID;
    static const uint32_t TYPE_ENGINE;
    static const uint32_t TYPE_EQUIPMENT;
    static const uint32_t TYPE_GOODS;
    static const uint32_t TYPE_HABITABLEPLANET;
    static const uint32_t TYPE_HULL;
    static const uint32_t TYPE_ITEM;
    static const uint32_t TYPE_INTERACTION_CONTEXT;
    static const uint32_t TYPE_LAND_CONTEXT;
    static const uint32_t TYPE_ASTEROID;
    static const uint32_t TYPE_MICROMODULUS;
    static const uint32_t TYPE_PLANET;
    static const uint32_t TYPE_RADAR;
    static const uint32_t TYPE_SCANNER;
    static const uint32_t TYPE_SHIP;
    static const uint32_t TYPE_SHIP_CONTEXT;
    static const uint32_t TYPE_PLANETARYSYSTEM;
    static const uint32_t TYPE_SPACEBASE;
    static const uint32_t TYPE_SPACEOBJECT;
    static const uint32_t TYPE_SYSTEMOBJECT;
    static const uint32_t TYPE_TANK;
    static const uint32_t TYPE_WEAPON;
    static const uint32_t TYPE_WORLDOBJECT;

    static boost::shared_ptr<WorldObject> createObjectByType(uint32_t type);

    static bool serializeTextureRegion(const TextureRegionDescriptor& region, std::ostream& stream);
    static bool deserializeTextureRegion(TextureRegionDescriptor& region, std::istream& stream);

    static bool serializeString(const std::string& str, std::ostream& stream);
    static bool deserializeString(std::string& str, std::istream& stream);
};
}
}

#endif // WORLDHELPER_H
