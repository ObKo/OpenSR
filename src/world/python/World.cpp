/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#include <boost/python.hpp>

namespace Rangers
{
namespace World
{
namespace Python
{
void exportWorldObject();
void exportItem();
void exportPlanetarySystem();
void exportSpaceObject();
void exportArtifact();
void exportEquipment();
void exportGoods();
void exportMicromodulus();
void exportShip();
void exportSystemObject();
void exportCargoHook();
void exportDefenceGenerator();
void exportDroid();
void exportEngine();
void exportHull();
void exportRadar();
void exportScanner();
void exportTank();
void exportWeapon();
void exportMeteor();
void exportPlanet();
void exportSpaceBase();
void exportDesertPlanet();
void exportHabitablePlanet();
void exportInteractionContext();
void exportShipContext();
void exportLandContext();
void exportPlanetManager();
void exportInfoWidget();
void exportSystemPlanetWidget();
void exportSystemWidget();
void exportTypes();
void exportWorldGenHook();
void exportWorldManager();
void exportWorldSkinManager();
void exportRaceManager();
void exportSystemManager();
void exportStyleManager();
}
}
}

BOOST_PYTHON_MODULE(_World)
{
    Rangers::World::Python::exportWorldObject();
    Rangers::World::Python::exportItem();
    Rangers::World::Python::exportPlanetarySystem();
    Rangers::World::Python::exportSpaceObject();
    Rangers::World::Python::exportArtifact();
    Rangers::World::Python::exportEquipment();
    Rangers::World::Python::exportGoods();
    Rangers::World::Python::exportMicromodulus();
    Rangers::World::Python::exportShip();
    Rangers::World::Python::exportSystemObject();
    Rangers::World::Python::exportCargoHook();
    Rangers::World::Python::exportDefenceGenerator();
    Rangers::World::Python::exportDroid();
    Rangers::World::Python::exportEngine();
    Rangers::World::Python::exportHull();
    Rangers::World::Python::exportRadar();
    Rangers::World::Python::exportScanner();
    Rangers::World::Python::exportTank();
    Rangers::World::Python::exportWeapon();
    Rangers::World::Python::exportMeteor();
    Rangers::World::Python::exportPlanet();
    Rangers::World::Python::exportSpaceBase();
    Rangers::World::Python::exportDesertPlanet();
    Rangers::World::Python::exportHabitablePlanet();
    Rangers::World::Python::exportInteractionContext();
    Rangers::World::Python::exportShipContext();
    Rangers::World::Python::exportLandContext();
    Rangers::World::Python::exportPlanetManager();
    Rangers::World::Python::exportInfoWidget();
    Rangers::World::Python::exportSystemPlanetWidget();
    Rangers::World::Python::exportSystemWidget();
    Rangers::World::Python::exportTypes();
    Rangers::World::Python::exportWorldGenHook();
    Rangers::World::Python::exportWorldManager();
    Rangers::World::Python::exportWorldSkinManager();
    Rangers::World::Python::exportRaceManager();
    Rangers::World::Python::exportSystemManager();
    Rangers::World::Python::exportStyleManager();
}
