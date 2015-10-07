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

#include "WorldObject.h"
#include "WorldContext.h"
#include "Race.h"
#include "Item.h"
#include "Goods.h"
#include "Equipment.h"
#include "Micromodulus.h"
#include "Artefact.h"
#include "Hull.h"
#include "Engine.h"
#include "Tank.h"
#include "Droid.h"
#include "CargoHook.h"
#include "DefenceGenerator.h"
#include "Radar.h"
#include "Scanner.h"
#include "Weapon.h"
#include "Sector.h"
#include "PlanetarySystem.h"
#include "SpaceObject.h"
#include "Container.h"
#include "Asteroid.h"
#include "Planet.h"
#include "MannedObject.h"
#include "InhabitedPlanet.h"
#include "DesertPlanet.h"
#include "Ship.h"
#include "SpaceStation.h"

//#include "ScriptPrototypes.h"

namespace OpenSR
{
namespace World
{
static QObject* managerSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    QQmlEngine::setObjectOwnership(WorldManager::instance(), QQmlEngine::CppOwnership);
    return WorldManager::instance();
}

void bindWorldTypes(QJSEngine *script, QQmlEngine *qml)
{
    qmlRegisterSingletonType<WorldManager>("OpenSR.World", 1, 0, "WorldManager", managerSingletonProvider);
    qmlRegisterType<WorldObject>("OpenSR.World", 1, 0, "WorldObject");
    qmlRegisterType<WorldContext>("OpenSR.World", 1, 0, "WorldContext");
    qRegisterMetaType<RaceStyle>();
    qRegisterMetaTypeStreamOperators<RaceStyle>();
    qmlRegisterType<Race>("OpenSR.World", 1, 0, "Race");
    // Have to rename Item, because there is bug in namespaces: QTBUG-10822
    qmlRegisterType<Item>("OpenSR.World", 1, 0, "WorldItem");
    qmlRegisterType<Goods>("OpenSR.World", 1, 0, "Goods");
    qmlRegisterType<Equipment>("OpenSR.World", 1, 0, "Equipment");
    qmlRegisterType<Micromodulus>("OpenSR.World", 1, 0, "Micromodulus");
    qmlRegisterType<Artefact>("OpenSR.World", 1, 0, "Artefact");
    qmlRegisterType<Hull>("OpenSR.World", 1, 0, "Hull");
    qmlRegisterType<OpenSR::World::Engine>("OpenSR.World", 1, 0, "Engine");
    qmlRegisterType<Tank>("OpenSR.World", 1, 0, "Tank");
    qmlRegisterType<Droid>("OpenSR.World", 1, 0, "Droid");
    qmlRegisterType<CargoHook>("OpenSR.World", 1, 0, "CargoHook");
    qmlRegisterType<DefenceGenerator>("OpenSR.World", 1, 0, "DefenceGenerator");
    qmlRegisterType<Radar>("OpenSR.World", 1, 0, "Radar");
    qmlRegisterType<Scanner>("OpenSR.World", 1, 0, "Scanner");
    qmlRegisterType<Weapon>("OpenSR.World", 1, 0, "Weapon");
    qmlRegisterType<Sector>("OpenSR.World", 1, 0, "Sector");
    qRegisterMetaType<PlanetarySystemStyle>();
    qRegisterMetaTypeStreamOperators<PlanetarySystemStyle>();
    qmlRegisterType<PlanetarySystem>("OpenSR.World", 1, 0, "PlanetarySystem");
    qmlRegisterType<SpaceObject>("OpenSR.World", 1, 0, "SpaceObject");
    qmlRegisterType<Container>("OpenSR.World", 1, 0, "Container");
    qmlRegisterType<Asteroid>("OpenSR.World", 1, 0, "Asteroid");
    qmlRegisterType<Planet>("OpenSR.World", 1, 0, "Planet");
    qmlRegisterType<MannedObject>("OpenSR.World", 1, 0, "MannedObject");
    qmlRegisterType<InhabitedPlanet>("OpenSR.World", 1, 0, "InhabitedPlanet");
    qmlRegisterType<DesertPlanet>("OpenSR.World", 1, 0, "DesertPlanet");
    qmlRegisterType<Ship>("OpenSR.World", 1, 0, "Ship");
    qmlRegisterType<SpaceStation>("OpenSR.World", 1, 0, "SpaceStation");

    script->globalObject().setProperty("world", script->newQObject(WorldManager::instance()));
    script->globalObject().setProperty("World", script->newQObject(new WorldJSFactory(script)));
}

WorldJSFactory::WorldJSFactory(QObject *parent): QObject(parent)
{
}

WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldJSFactory, RaceStyle)
WORLD_JS_DEFAULT_GADGET_CONSTRUCTOR(WorldJSFactory, PlanetarySystemStyle)

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
