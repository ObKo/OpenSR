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

#include "WorldObject.h"

namespace Rangers
{
namespace World
{
bool WorldObject::deserialize(std::istream& stream)
{
    return true;
}

uint64_t WorldObject::id() const
{
    return m_objectID;
}

bool WorldObject::serialize(std::ostream& stream) const
{
    return true;
}

uint32_t WorldObject::type() const
{
    return 0x1b90a69f;
}

WorldObject* WorldObject::objectByType(uint32_t type)
{
    WorldObject *object;
    switch (type)
    {
    case 0x17436ae1:
        object = new Artifact();
        break;
    case 0x928ea67:
        object = new CargoHook();
        break;
    case 0x29ba7a36:
        object = new DefenceGenerator();
        break;
    case 0x116adcbb:
        object = new DesertPlanet();
        break;
    case 0x7c48b92c:
        object = new Droid();
        break;
    case 0x756eea2f:
        object = new Engine();
        break;
    case 0x3209e044:
        object = new Equipment();
        break;
    case 0x6d261f94:
        object = new Goods();
        break;
    case 0x79f89bfa:
        object = new HabitablePlanet();
        break;
    case 0x3938e105:
        object = new Hull();
        break;
    case 0x4b74e518:
        object = new Item();
        break;
    case 0x71691dfd:
        object = new Meteor();
        break;
    case 0x6921522f:
        object = new Micromodulus();
        break;
    case 0x79105c84:
        object = new Planet();
        break;
    case 0x5fcf4370:
        object = new Radar();
        break;
    case 0x28ff1510:
        object = new Scanner();
        break;
    case 0x2b9a8c11:
        object = new Ship();
        break;
    case 0x1c832836:
        object = new SolarSystem();
        break;
    case 0x606e1534:
        object = new SpaceBase();
        break;
    case 0x44357eaf:
        object = new SpaceObject();
        break;
    case 0x127dbfab:
        object = new SystemObject();
        break;
    case 0x22980928:
        object = new Tank();
        break;
    case 0x2ff422f6:
        object = new Weapon();
        break;
    case 0x1b90a69f:
        object = new WorldObject();
        break;
    }
    return object;
}
}
}
