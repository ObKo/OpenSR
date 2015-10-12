/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#ifndef OPENSR_WORLD_WORLDBINDINGS_H
#define OPENSR_WORLD_WORLDBINDINGS_H

#include "WorldObject.h"

#include "PlanetarySystem.h"
#include "Race.h"
#include "Asteroid.h"

class QJSEngine;
class QQmlEngine;

#define WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Class) Q_INVOKABLE QObject* Class(WorldObject *parent = 0);
#define WORLD_DECLARE_JS_GADGET_CONSTRUCTOR(Class) Q_INVOKABLE OpenSR::World::Class Class();

#define WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(Factory, Class)\
    QObject* Factory::Class(WorldObject *parent) {\
        QObject *obj = (QObject*)WorldObject::createObject<World::Class>(parent);\
        QQmlEngine::setObjectOwnership(obj, QQmlEngine::JavaScriptOwnership);\
        return obj;\
    }

#define WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(Factory, Class)\
    Class Factory::Class() {\
        return World::Class();\
    }

namespace OpenSR
{
namespace World
{
void bindWorldTypes(QJSEngine *script, QQmlEngine *qml);

class WorldJSFactory : public QObject
{
    Q_OBJECT
public:
    WorldJSFactory(QObject *parent = 0);

    WORLD_DECLARE_JS_GADGET_CONSTRUCTOR(RaceStyle)
    WORLD_DECLARE_JS_GADGET_CONSTRUCTOR(PlanetarySystemStyle)
    WORLD_DECLARE_JS_GADGET_CONSTRUCTOR(AsteroidStyle)

    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Race)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Item)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Goods)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Equipment)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Micromodulus)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Artefact)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Hull)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Engine)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Tank)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Droid)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(CargoHook)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(DefenceGenerator)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Radar)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Scanner)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Weapon)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Sector)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(PlanetarySystem)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(SpaceObject)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Container)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Asteroid)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Planet)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(MannedObject)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(InhabitedPlanet)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(DesertPlanet)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(Ship)
    WORLD_DECLARE_JS_OBJECT_CONSTRUCTOR(SpaceStation)
};
}
}

#endif // OPENSR_WORLD_WORLDBINDINGS_H
