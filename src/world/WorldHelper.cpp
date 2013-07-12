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

#include "WorldHelper.h"

#include <libRanger.h>

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
#include "Meteor.h"
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

namespace Rangers
{
namespace World
{
WorldObject* WorldHelper::objectByType(uint32_t type)
{
    WorldObject *object;
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
    case TYPE_ITEM:
        object = new Item();
        break;
    case TYPE_METEOR:
        object = new Meteor();
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
    return object;
}

bool WorldHelper::serializeString(const std::wstring& str, std::ostream& stream)
{
    std::string str_ = toUTF8(str);
    return serializeString(str_, stream);
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

bool WorldHelper::deserializeString(std::wstring& str, std::istream& stream)
{
    uint32_t l;
    char *s;

    stream.read((char *)&l, 4);
    if (!stream.good())
        return false;
    s = new char[l + 1];
    stream.read(s, l);
    if (!stream.good())
        return false;
    s[l] = '\0';
    str = fromUTF8(s, l);
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
        return false;
    s[l] = '\0';
    str = std::string(s, l);
    return true;
}
}
}
