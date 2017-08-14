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

#ifndef OPENSR_WORLD_WORLDMANAGER_H
#define OPENSR_WORLD_WORLDMANAGER_H

#include "World.h"

#include <QObject>
#include <QAbstractAnimation>

#include "WorldContext.h"
#include "WorldBindings.h"

namespace OpenSR
{
namespace World
{
class OPENSR_WORLD_API TurnAnimation: public QAbstractAnimation
{
    Q_OBJECT
public:
    TurnAnimation(QObject *parent = 0);

    virtual int	duration() const;
    virtual void updateCurrentTime(int currentTime);
};

class Race;
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
class Asteroid;
class Planet;
class MannedObject;
class InhabitedPlanet;
class DesertPlanet;
class Ship;
class SpaceStation;

class OPENSR_WORLD_API WorldManager: public QObject
{
    Q_OBJECT
    Q_PROPERTY(WorldContext* context READ context NOTIFY contextChanged)

public:
    WorldManager(QObject *parent = 0);
    virtual ~WorldManager();

    quint32 getNextId() const;

    static WorldManager *instance();

    WorldContext* context() const;

    Q_INVOKABLE QString typeName(quint32 type) const;

    Q_INVOKABLE void generateWorld(const QString& genScriptUrl);
    Q_INVOKABLE bool saveWorld(const QString& path);
    Q_INVOKABLE bool loadWorld(const QString& path);

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

Q_SIGNALS:
    void contextChanged();

public Q_SLOTS:
    void startTurn();
    void finishTurn();

private:
    WorldContext* m_context;
    TurnAnimation *m_animation;

    static WorldManager* m_staticInstance;
    static quint32 m_idPool;

    Q_DISABLE_COPY(WorldManager)
};
}
}

#endif // OPENSR_WORLD_WORLDMANAGER_H
