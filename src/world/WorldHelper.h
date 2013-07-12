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

#ifndef WORLD_WORLDHELPER_H
#define WORLD_WORLDHELPER_H

#include <stdint.h>

namespace Rangers
{
namespace World
{
class WorldObject;
class WorldHelper
{
public:
    static const uint32_t TYPE_ARTIFACT = 0x17436ae1;
    static const uint32_t TYPE_CARGOHOOK = 0x928ea67;
    static const uint32_t TYPE_DEFENCEGENERATOR = 0x29ba7a36;
    static const uint32_t TYPE_DESERTPLANET = 0x116adcbb;
    static const uint32_t TYPE_DROID = 0x7c48b92c;
    static const uint32_t TYPE_ENGINE = 0x756eea2f;
    static const uint32_t TYPE_EQUIPMENT = 0x3209e044;
    static const uint32_t TYPE_GOODS = 0x6d261f94;
    static const uint32_t TYPE_HABITABLEPLANET = 0x79f89bfa;
    static const uint32_t TYPE_HULL = 0x3938e105;
    static const uint32_t TYPE_ITEM = 0x4b74e518;
    static const uint32_t TYPE_METEOR = 0x71691dfd;
    static const uint32_t TYPE_MICROMODULUS = 0x6921522f;
    static const uint32_t TYPE_PLANET = 0x79105c84;
    static const uint32_t TYPE_RADAR = 0x5fcf4370;
    static const uint32_t TYPE_SCANNER = 0x28ff1510;
    static const uint32_t TYPE_SHIP = 0x2b9a8c11;
    static const uint32_t TYPE_PLANETARYSYSTEM = 0x1c832836;
    static const uint32_t TYPE_SPACEBASE = 0x606e1534;
    static const uint32_t TYPE_SPACEOBJECT = 0x44357eaf;
    static const uint32_t TYPE_SYSTEMOBJECT = 0x127dbfab;
    static const uint32_t TYPE_TANK = 0x22980928;
    static const uint32_t TYPE_WEAPON = 0x2ff422f6;
    static const uint32_t TYPE_WORLDOBJECT = 0x1b90a69f;

    static WorldObject* objectByType(uint32_t type);
};
}
}

#endif // WORLDHELPER_H
