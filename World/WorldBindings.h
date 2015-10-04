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

#include "Race.h"
#include "PlanetarySystem.h"

class QJSEngine;
class QQmlEngine;

#define WORLD_DECLARE_JS_GADGET_CONSTRUCTOR_ARGS(Class, ...) Q_INVOKABLE OpenSR::World::Class Class(__VA_ARGS__);
#define WORLD_DECLARE_JS_CONSTRUCTOR_ARGS(Class, ...) Q_INVOKABLE QObject* Class(__VA_ARGS__);
#define WORLD_DECLARE_JS_CONSTRUCTOR(Class) WORLD_DECLARE_JS_CONSTRUCTOR_ARGS(Class, QObject *parent = 0)

#define WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(Factory, Class)\
    QObject* Factory::Class(QObject *parent) {\
        QObject *obj = new World::Class(qobject_cast<WorldObject*>(parent));\
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
class WorldObject;

void bindWorldTypes(QJSEngine *script, QQmlEngine *qml);

class WorldJSFactory : public QObject
{
    Q_OBJECT
public:
    WorldJSFactory(QObject *parent = 0);

    WORLD_DECLARE_JS_GADGET_CONSTRUCTOR_ARGS(RaceStyle)
    WORLD_DECLARE_JS_GADGET_CONSTRUCTOR_ARGS(PlanetarySystemStyle)

    WORLD_DECLARE_JS_CONSTRUCTOR(Race)
    WORLD_DECLARE_JS_CONSTRUCTOR(Item)
    WORLD_DECLARE_JS_CONSTRUCTOR(Goods)
    WORLD_DECLARE_JS_CONSTRUCTOR(Equipment)
    WORLD_DECLARE_JS_CONSTRUCTOR(Micromodulus)
    WORLD_DECLARE_JS_CONSTRUCTOR(Artefact)
    WORLD_DECLARE_JS_CONSTRUCTOR(Hull)
    WORLD_DECLARE_JS_CONSTRUCTOR(Engine)
    WORLD_DECLARE_JS_CONSTRUCTOR(Tank)
    WORLD_DECLARE_JS_CONSTRUCTOR(Droid)
    WORLD_DECLARE_JS_CONSTRUCTOR(CargoHook)
    WORLD_DECLARE_JS_CONSTRUCTOR(DefenceGenerator)
    WORLD_DECLARE_JS_CONSTRUCTOR(Radar)
    WORLD_DECLARE_JS_CONSTRUCTOR(Scanner)
    WORLD_DECLARE_JS_CONSTRUCTOR(Weapon)
    WORLD_DECLARE_JS_CONSTRUCTOR(Sector)
    WORLD_DECLARE_JS_CONSTRUCTOR(PlanetarySystem)
    WORLD_DECLARE_JS_CONSTRUCTOR(SpaceObject)
    WORLD_DECLARE_JS_CONSTRUCTOR(Container)
    WORLD_DECLARE_JS_CONSTRUCTOR(Asteroid)
    WORLD_DECLARE_JS_CONSTRUCTOR(Planet)
    WORLD_DECLARE_JS_CONSTRUCTOR(MannedObject)
    WORLD_DECLARE_JS_CONSTRUCTOR(InhabitedPlanet)
    WORLD_DECLARE_JS_CONSTRUCTOR(DesertPlanet)
    WORLD_DECLARE_JS_CONSTRUCTOR(Ship)
    WORLD_DECLARE_JS_CONSTRUCTOR(SpaceStation)
};
}
}

#endif // OPENSR_WORLD_WORLDBINDINGS_H
