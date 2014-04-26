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

#include "WorldHelper.h"

#include <OpenSR/libRanger.h>

#include "Artifact.h"
#include "CargoHook.h"
#include "DefenceGenerator.h"
#include "DesertPlanet.h"
#include "Droid.h"
#include "Engine.h"
#include "Equipment.h"
#include "Goods.h"
#include "HabitablePlanet.h"
#include "Hull.h"
#include "InteractionContext.h"
#include "Item.h"
#include "LandContext.h"
#include "Asteroid.h"
#include "Micromodulus.h"
#include "Planet.h"
#include "Radar.h"
#include "Scanner.h"
#include "Ship.h"
#include "ShipContext.h"
#include "PlanetarySystem.h"
#include "SpaceBase.h"
#include "SpaceObject.h"
#include "SystemObject.h"
#include "Tank.h"
#include "Types.h"
#include "Weapon.h"
#include "WorldObject.h"
#include "InteractionContext.h"
#include "LandContext.h"
#include "ShipContext.h"
#include <OpenSR/Types.h>

namespace
{
const uint32_t TEXTURE_REGION_SIGNATURE = *((uint32_t*)"SRTR");
}

namespace Rangers
{
namespace World
{
const uint32_t WorldHelper::TYPE_ARTIFACT = 0x17436ae1;
const uint32_t WorldHelper::TYPE_CARGOHOOK = 0x928ea67;
const uint32_t WorldHelper::TYPE_DEFENCEGENERATOR = 0x29ba7a36;
const uint32_t WorldHelper::TYPE_DESERTPLANET = 0x116adcbb;
const uint32_t WorldHelper::TYPE_DROID = 0x7c48b92c;
const uint32_t WorldHelper::TYPE_ENGINE = 0x756eea2f;
const uint32_t WorldHelper::TYPE_EQUIPMENT = 0x3209e044;
const uint32_t WorldHelper::TYPE_GOODS = 0x6d261f94;
const uint32_t WorldHelper::TYPE_HABITABLEPLANET = 0x79f89bfa;
const uint32_t WorldHelper::TYPE_HULL = 0x3938e105;
const uint32_t WorldHelper::TYPE_ITEM = 0x4b74e518;
const uint32_t WorldHelper::TYPE_INTERACTION_CONTEXT = 0xf62886ad;
const uint32_t WorldHelper::TYPE_LAND_CONTEXT = 0x9b5430be;
const uint32_t WorldHelper::TYPE_ASTEROID = 0x71691dfd;
const uint32_t WorldHelper::TYPE_MICROMODULUS = 0x6921522f;
const uint32_t WorldHelper::TYPE_PLANET = 0x79105c84;
const uint32_t WorldHelper::TYPE_RADAR = 0x5fcf4370;
const uint32_t WorldHelper::TYPE_SCANNER = 0x28ff1510;
const uint32_t WorldHelper::TYPE_SHIP = 0x2b9a8c11;
const uint32_t WorldHelper::TYPE_SHIP_CONTEXT = 0x3e572751;
const uint32_t WorldHelper::TYPE_PLANETARYSYSTEM = 0x1c832836;
const uint32_t WorldHelper::TYPE_SPACEBASE = 0x606e1534;
const uint32_t WorldHelper::TYPE_SPACEOBJECT = 0x44357eaf;
const uint32_t WorldHelper::TYPE_SYSTEMOBJECT = 0x127dbfab;
const uint32_t WorldHelper::TYPE_TANK = 0x22980928;
const uint32_t WorldHelper::TYPE_WEAPON = 0x2ff422f6;
const uint32_t WorldHelper::TYPE_WORLDOBJECT = 0x1b90a69f;

boost::shared_ptr<WorldObject> WorldHelper::createObjectByType(uint32_t type)
{
    WorldObject *object = 0;
    switch (type)
    {
    case TYPE_ARTIFACT:
        object = new Artifact();
        break;
    case TYPE_CARGOHOOK:
        object = new CargoHook();
        break;
    case TYPE_DEFENCEGENERATOR:
        object = new DefenceGenerator();
        break;
    case TYPE_DESERTPLANET:
        object = new DesertPlanet();
        break;
    case TYPE_DROID:
        object = new Droid();
        break;
    case TYPE_ENGINE:
        object = new Engine();
        break;
    case TYPE_EQUIPMENT:
        object = new Equipment();
        break;
    case TYPE_GOODS:
        object = new Goods();
        break;
    case TYPE_HABITABLEPLANET:
        object = new HabitablePlanet();
        break;
    case TYPE_HULL:
        object = new Hull();
        break;
    case TYPE_INTERACTION_CONTEXT:
        object = new InteractionContext();
        break;
    case TYPE_LAND_CONTEXT:
        object = new LandContext();
        break;
    case TYPE_ITEM:
        object = new Item();
        break;
    case TYPE_ASTEROID:
        object = new Asteroid();
        break;
    case TYPE_MICROMODULUS:
        object = new Micromodulus();
        break;
    case TYPE_PLANET:
        object = new Planet();
        break;
    case TYPE_RADAR:
        object = new Radar();
        break;
    case TYPE_SCANNER:
        object = new Scanner();
        break;
    case TYPE_SHIP:
        object = new Ship();
        break;
    case TYPE_SHIP_CONTEXT:
        object = new ShipContext();
        break;
    case TYPE_PLANETARYSYSTEM:
        object = new PlanetarySystem();
        break;
    case TYPE_SPACEBASE:
        object = new SpaceBase();
        break;
    case TYPE_SPACEOBJECT:
        object = new SpaceObject();
        break;
    case TYPE_SYSTEMOBJECT:
        object = new SystemObject();
        break;
    case TYPE_TANK:
        object = new Tank();
        break;
    case TYPE_WEAPON:
        object = new Weapon();
        break;
    case TYPE_WORLDOBJECT:
        object = new WorldObject();
        break;
    }
    return boost::shared_ptr<WorldObject>(object);
}

bool WorldHelper::serializeString(const std::string& str, std::ostream& stream)
{
    uint32_t l = str.length();

    stream.write((const char *)&l, 4);
    stream.write(str.c_str(), l);

    if (!stream.good())
        return false;

    return true;
}

bool WorldHelper::deserializeString(std::string& str, std::istream& stream)
{
    uint32_t l;
    char *s;

    stream.read((char *)&l, 4);
    if (!stream.good())
        return false;
    s = new char[l + 1];
    stream.read(s, l);
    if (!stream.good())
    {
        delete[] s;
        return false;
    }
    s[l] = '\0';
    str = std::string(s, l);
    delete[] s;
    return true;
}

bool WorldHelper::deserializeTextureRegion(TextureRegionDescriptor& region, std::istream& stream)
{
    int32_t x, y, width, height;
    uint32_t sig;

    stream.read((char *)&sig, 4);

    if (sig != TEXTURE_REGION_SIGNATURE)
        return false;

    if (!deserializeString(region.texture, stream))
        return false;

    stream.read((char*)&x, 4);
    stream.read((char*)&y, 4);
    stream.read((char*)&width, 4);
    stream.read((char*)&height, 4);

    if (!stream.good())
        return false;

    region.x = x;
    region.y = y;
    region.width = width;
    region.height = height;

    return true;
}

bool WorldHelper::serializeTextureRegion(const TextureRegionDescriptor& region, std::ostream& stream)
{
    int32_t x = region.x, y = region.y, width = region.width, height = region.height;

    stream.write((const char *)&TEXTURE_REGION_SIGNATURE, 4);

    if (!stream.good())
        return false;

    if (!serializeString(region.texture, stream))
        return false;

    stream.write((const char*)&x, 4);
    stream.write((const char*)&y, 4);
    stream.write((const char*)&width, 4);
    stream.write((const char*)&height, 4);

    if (!stream.good())
        return false;

    return true;
}
}
}
