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

#include <OpenSR/PluginInterface.h>
#include <OpenSR/ResourceManager.h>
#include <version.h>

/*
#include "WorldObject.h"
#include "SolarSystem.h"
#include "Item.h"
#include "SpaceObject.h"
#include "Artifact.h"
#include "Equipment.h"
#include "Goods.h"
#include "Micromodulus.h"
#include "Ship.h"
#include "SystemObject.h"
#include "CargoHook.h"
#include "DefenceGenerator.h"
#include "Droid.h"
#include "Engine.h"
#include "Hull.h"
#include "Radar.h"
#include "Scanner.h"
#include "Tank.h"
#include "Weapon.h"
#include "Meteor.h"
#include "Planet.h"
#include "SpaceBase.h"
#include "DesertPlanet.h"
#include "HabitablePlanet.h"
#include "InteractionContext.h"
#include "LandContext.h"
#include "ShipContext.h"
#include "WorldHelper.h"
#include "WorldManager.h"
#include "SystemWidget.h"*/
#include "Types.h"

using namespace Rangers::World;

int rangersAPIVersion()
{
    return RANGERS_API_VERSION;
}

int rangersPluginInit()
{
    return 0;
}

void rangersPluginDeinit()
{
}