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

#include "WorldBindings.h"

#include <QJSEngine>
#include <QQmlEngine>
#include <QtQml>

#include "WorldManager.h"

#include "PlanetarySystem.h"
#include "Race.h"
#include "Asteroid.h"

namespace OpenSR
{
namespace World
{
class Item;
class Goods;
class Equipment;
class Micromodulus;
class Artefact;
class Hull;
class Engine;
class Tank;
class Droid;
class CargoHook;
class DefenceGenerator;
class Radar;
class Scanner;
class Weapon;
class Sector;
class SpaceObject;
class Container;
class Planet;
class MannedObject;
class InhabitedPlanet;
class DesertPlanet;
class Ship;
class SpaceStation;
class ResourceManager;

static QObject* managerSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    QQmlEngine::setObjectOwnership(WorldManager::instance(), QQmlEngine::CppOwnership);
    return WorldManager::instance();
}

void bindWorldTypes(QJSEngine *script, QQmlEngine *qml)
{
    qmlRegisterSingletonType<WorldManager>("OpenSR.World", 1, 0, "WorldManager", managerSingletonProvider);
    WorldObject::registerType<WorldObject>();
    WorldObject::registerType<WorldContext>();
    WorldObject::registerType<Race>();
    WorldObject::registerType<Item>();
    WorldObject::registerType<Goods>();
    WorldObject::registerType<Equipment>();
    WorldObject::registerType<Micromodulus>();
    WorldObject::registerType<Artefact>();
    WorldObject::registerType<Hull>();
    WorldObject::registerType<OpenSR::World::Engine>();
    WorldObject::registerType<Tank>();
    WorldObject::registerType<Droid>();
    WorldObject::registerType<CargoHook>();
    WorldObject::registerType<DefenceGenerator>();
    WorldObject::registerType<Radar>();
    WorldObject::registerType<Scanner>();
    WorldObject::registerType<Weapon>();
    WorldObject::registerType<Sector>();
    WorldObject::registerType<PlanetarySystem>();
    WorldObject::registerType<SpaceObject>();
    WorldObject::registerType<Container>();
    WorldObject::registerType<Asteroid>();
    WorldObject::registerType<Planet>();
    WorldObject::registerType<MannedObject>();
    WorldObject::registerType<InhabitedPlanet>();
    WorldObject::registerType<DesertPlanet>();
    WorldObject::registerType<Ship>();
    WorldObject::registerType<SpaceStation>();
    WorldObject::registerType<ResourceManager>();

    script->globalObject().setProperty("world", script->newQObject(WorldManager::instance()));
    script->globalObject().setProperty("World", script->newQObject(new WorldJSFactory(script)));
}

WorldJSFactory::WorldJSFactory(QObject *parent): QObject(parent)
{
}

WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldJSFactory, RaceStyle)
WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldJSFactory, PlanetarySystemStyle)
WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldJSFactory, AsteroidStyle)

WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Race)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Item)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Goods)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Equipment)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Micromodulus)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Artefact)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Hull)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Engine)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Tank)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Droid)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, CargoHook)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, DefenceGenerator)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Radar)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Scanner)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Weapon)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Sector)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, PlanetarySystem)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, SpaceObject)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Container)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Asteroid)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Planet)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, MannedObject)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, InhabitedPlanet)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, DesertPlanet)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, Ship)
WORLD_JS_DEFAULT_OBJECT_CONSTRUCTOR(WorldJSFactory, SpaceStation)
}
}
