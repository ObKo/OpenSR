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

#include <QScriptEngine>
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

Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::WorldObject, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::WorldContext, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Race, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Item, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Goods, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Equipment, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Micromodulus, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Artefact, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Hull, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Engine, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Tank, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Droid, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::CargoHook, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::DefenceGenerator, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Radar, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Scanner, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Weapon, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Sector, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::PlanetarySystem, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::SpaceObject, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Container, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Asteroid, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Planet, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::MannedObject, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::InhabitedPlanet, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::DesertPlanet, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::Ship, OpenSR::World::WorldObject*);
Q_SCRIPT_DECLARE_QMETAOBJECT(OpenSR::World::SpaceStation, OpenSR::World::WorldObject*);

namespace OpenSR
{
namespace World
{
QScriptValue raceStyleToScriptValue(QScriptEngine *engine, const RaceStyle &s)
{
    QScriptValue obj = engine->newObject();
    obj.setProperty("icon", s.icon);
    obj.setProperty("color", s.color.name());
    obj.setProperty("sound", s.sound);
    return obj;
}

void raceStyleFromScriptValue(const QScriptValue &obj, RaceStyle &s)
{
    s.color = QColor(obj.property("color").toString());
    s.icon = obj.property("icon").toString();
    s.sound = obj.property("sound").toString();
}

static QObject* managerSingletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    QQmlEngine::setObjectOwnership(WorldManager::instance(), QQmlEngine::CppOwnership);
    return WorldManager::instance();
}

void bindWorldTypes(QScriptEngine *script, QQmlEngine *qml)
{
    qmlRegisterSingletonType<WorldManager>("OpenSR.World", 1, 0, "WorldManager", managerSingletonProvider);
    qmlRegisterType<WorldObject>("OpenSR.World", 1, 0, "WorldObject");
    qmlRegisterType<WorldContext>("OpenSR.World", 1, 0, "WorldContext");
    qmlRegisterType<Race>("OpenSR.World", 1, 0, "Race");
    qmlRegisterType<Item>("OpenSR.World", 1, 0, "Item");
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

    QScriptValue world = script->newObject();

    world.setProperty("Manager", script->newQObject(WorldManager::instance()));

    world.setProperty("WorldObject", script->scriptValueFromQMetaObject<WorldObject>());
    world.setProperty("WorldContext", script->scriptValueFromQMetaObject<WorldContext>());
    world.setProperty("Race", script->scriptValueFromQMetaObject<Race>());
    world.setProperty("Item", script->scriptValueFromQMetaObject<Item>());
    world.setProperty("Goods", script->scriptValueFromQMetaObject<Goods>());
    world.setProperty("Equipment", script->scriptValueFromQMetaObject<Equipment>());
    world.setProperty("Micromodulus", script->scriptValueFromQMetaObject<Micromodulus>());
    world.setProperty("Artefact", script->scriptValueFromQMetaObject<Artefact>());
    world.setProperty("Hull", script->scriptValueFromQMetaObject<Hull>());
    world.setProperty("Engine", script->scriptValueFromQMetaObject<Engine>());
    world.setProperty("Tank", script->scriptValueFromQMetaObject<Tank>());
    world.setProperty("Droid", script->scriptValueFromQMetaObject<Droid>());
    world.setProperty("CargoHook", script->scriptValueFromQMetaObject<CargoHook>());
    world.setProperty("DefenceGenerator", script->scriptValueFromQMetaObject<DefenceGenerator>());
    world.setProperty("Radar", script->scriptValueFromQMetaObject<Radar>());
    world.setProperty("Scanner", script->scriptValueFromQMetaObject<Scanner>());
    world.setProperty("Weapon", script->scriptValueFromQMetaObject<Weapon>());
    world.setProperty("Sector", script->scriptValueFromQMetaObject<Sector>());
    world.setProperty("PlanetarySystem", script->scriptValueFromQMetaObject<PlanetarySystem>());
    world.setProperty("SpaceObject", script->scriptValueFromQMetaObject<SpaceObject>());
    world.setProperty("Container", script->scriptValueFromQMetaObject<Container>());
    world.setProperty("Asteroid", script->scriptValueFromQMetaObject<Asteroid>());
    world.setProperty("Planet", script->scriptValueFromQMetaObject<Planet>());
    world.setProperty("MannedObject", script->scriptValueFromQMetaObject<MannedObject>());
    world.setProperty("InhabitedPlanet", script->scriptValueFromQMetaObject<InhabitedPlanet>());
    world.setProperty("DesertPlanet", script->scriptValueFromQMetaObject<DesertPlanet>());
    world.setProperty("Ship", script->scriptValueFromQMetaObject<Ship>());
    world.setProperty("SpaceStation", script->scriptValueFromQMetaObject<SpaceStation>());

    qScriptRegisterMetaType(script, raceStyleToScriptValue, raceStyleFromScriptValue);

    script->globalObject().setProperty("World", world);
}
}
}
