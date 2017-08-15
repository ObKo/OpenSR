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

// TODO: Move Constructors to Separate factory?

#include "PlanetarySystem.h"
#include "Race.h"
#include "Asteroid.h"
#include "Planet.h"
#include "SpaceStation.h"

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

    // TODO: Move Constructors to Separate factory?
    Q_INVOKABLE OpenSR::World::RaceStyle            newRaceStyle();
    Q_INVOKABLE OpenSR::World::PlanetarySystemStyle newPlanetarySystemStyle();
    Q_INVOKABLE OpenSR::World::AsteroidStyle        newAsteroidStyle();
    Q_INVOKABLE OpenSR::World::PlanetStyle          newPlanetStyle();
    Q_INVOKABLE OpenSR::World::StationStyle         newStationStyle();

    Q_INVOKABLE OpenSR::World::Race*                newRace(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Item*                newItem(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Goods*               newGoods(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Equipment*           newEquipment(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Micromodulus*        newMicromodulus(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Artefact*            newArtefact(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Hull*                newHull(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Engine*              newEngine(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Tank*                newTank(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Droid*               newDroid(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::CargoHook*           newCargoHook(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::DefenceGenerator*    newDefenceGenerator(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Radar*               newRadar(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Scanner*             newScanner(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Weapon*              newWeapon(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Sector*              newSector(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::PlanetarySystem*     newPlanetarySystem(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::SpaceObject*         newSpaceObject(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Container*           newContainer(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Asteroid*            newAsteroid(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Planet*              newPlanet(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::MannedObject*        newMannedObject(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::InhabitedPlanet*     newInhabitedPlanet(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::DesertPlanet*        newDesertPlanet(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::Ship*                newShip(OpenSR::World::WorldObject *parent = 0);
    Q_INVOKABLE OpenSR::World::SpaceStation*        newSpaceStation(OpenSR::World::WorldObject *parent = 0);

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
