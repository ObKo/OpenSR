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
#include "WorldManager.h"

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
    script->globalObject().setProperty("World", script->newQObject(WorldManager::instance()));

    qmlRegisterSingletonType<WorldManager>("OpenSR.World", 1, 0, "WorldManager", managerSingletonProvider);
    WorldObject::registerType<WorldObject>(qml, script);
    WorldObject::registerType<WorldContext>(qml, script);
    WorldObject::registerType<Race>(qml, script);
    WorldObject::registerType<Item>(qml, script);
    WorldObject::registerType<Goods>(qml, script);
    WorldObject::registerType<Equipment>(qml, script);
    WorldObject::registerType<Micromodulus>(qml, script);
    WorldObject::registerType<Artefact>(qml, script);
    WorldObject::registerType<Hull>(qml, script);
    WorldObject::registerType<OpenSR::World::Engine>(qml, script);
    WorldObject::registerType<Tank>(qml, script);
    WorldObject::registerType<Droid>(qml, script);
    WorldObject::registerType<CargoHook>(qml, script);
    WorldObject::registerType<DefenceGenerator>(qml, script);
    WorldObject::registerType<Radar>(qml, script);
    WorldObject::registerType<Scanner>(qml, script);
    WorldObject::registerType<Weapon>(qml, script);
    WorldObject::registerType<Sector>(qml, script);
    WorldObject::registerType<PlanetarySystem>(qml, script);
    WorldObject::registerType<SpaceObject>(qml, script);
    WorldObject::registerType<Container>(qml, script);
    WorldObject::registerType<Asteroid>(qml, script);
    WorldObject::registerType<Planet>(qml, script);
    WorldObject::registerType<MannedObject>(qml, script);
    WorldObject::registerType<InhabitedPlanet>(qml, script);
    WorldObject::registerType<DesertPlanet>(qml, script);
    WorldObject::registerType<Ship>(qml, script);
    WorldObject::registerType<SpaceStation>(qml, script);
    WorldObject::registerType<ResourceManager>(qml, script);
}
}
}
