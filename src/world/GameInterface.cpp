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

#include <PluginInterface.h>
#include <ResourceManager.h>
#include <Log.h>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include "version.h"

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
#include "Types.h"
#include "WorldHelper.h"

using namespace luabind;
using namespace Rangers::World;

int rangersAPIVersion()
{
    return RANGERS_API_VERSION;
}

int rangersPluginInit()
{
    return 0;
}

void rangersPluginInitLua(lua_State *state)
{
    luabind::module(state) [
        namespace_("World")[
            luabind::class_<InteractionContext>("InteractionContext")
            .def(luabind::constructor<>())
            .def("relation", &InteractionContext::relation)
            .def("race", &InteractionContext::race),

            luabind::class_<LandContext, InteractionContext>("LandContext")
            .def(luabind::constructor<>()),

            luabind::class_<ShipContext, InteractionContext>("ShipContext")
            .def(luabind::constructor<>()),

            luabind::class_<Point>("Point")
            .def_readwrite("x", &Point::x)
            .def_readwrite("y", &Point::y),

            luabind::class_<WorldObject>("WorldObject")
            .def(luabind::constructor<>())
            .def("id", &WorldObject::id)
            .def("type", &WorldObject::type),

            luabind::class_<SolarSystem, WorldObject>("SolarSystem")
            .def(luabind::constructor<>())
            .def("name", &SolarSystem::name)
            .def("position", &SolarSystem::position)
            .def("systemObjects", &SolarSystem::systemObjects)
            .def("size", &SolarSystem::size),

            luabind::class_<Item, WorldObject>("Item")
            .def(luabind::constructor<>())
            .def("name", &Item::name)
            .def("size", &Item::size)
            .def("cost", &Item::cost),

            luabind::class_<SpaceObject, WorldObject>("SpaceObject")
            .def(luabind::constructor<>())
            .def("position", &SpaceObject::position),

            luabind::class_<Artifact, Item>("Artifact")
            .def(luabind::constructor<>()),

            luabind::class_<Equipment, Item>("Equipment")
            .def(luabind::constructor<>())
            .def("race", &Equipment::race),

            luabind::class_<Goods, Item>("Goods")
            .def(luabind::constructor<>())
            .def("quantity", &Goods::quantity)
            .def("price", &Goods::price),

            luabind::class_<Micromodulus, Item>("Micromodulus")
            .def(luabind::constructor<>()),

            luabind::class_<Ship, SpaceObject>("Ship")
            .def(luabind::constructor<>())
            .def("name", &Ship::name)
            .def("context", &Ship::context),

            luabind::class_<SystemObject, SpaceObject>("SystemObject")
            .def(luabind::constructor<>())
            .def("name", &SystemObject::name),

            luabind::class_<CargoHook, Equipment>("CargoHook")
            .def(luabind::constructor<>()),

            luabind::class_<DefenceGenerator, Equipment>("DefenceGenerator")
            .def(luabind::constructor<>()),

            luabind::class_<Droid, Equipment>("Droid")
            .def(luabind::constructor<>()),

            luabind::class_<Engine, Equipment>("Engine")
            .def(luabind::constructor<>()),

            luabind::class_<Hull, Equipment>("Hull")
            .def(luabind::constructor<>()),

            luabind::class_<Radar, Equipment>("Radar")
            .def(luabind::constructor<>()),

            luabind::class_<Scanner, Equipment>("Scanner")
            .def(luabind::constructor<>()),

            luabind::class_<Tank, Equipment>("Tank")
            .def(luabind::constructor<>()),

            luabind::class_<Weapon, Equipment>("Weapon")
            .def(luabind::constructor<>()),

            luabind::class_<Meteor, SystemObject>("Meteor")
            .def(luabind::constructor<>()),

            luabind::class_<Planet, SystemObject>("Planet")
            .def(luabind::constructor<>()),

            luabind::class_<SpaceBase, SystemObject>("SpaceBase")
            .def(luabind::constructor<>()),

            luabind::class_<DesertPlanet, Planet>("DesertPlanet")
            .def(luabind::constructor<>()),

            luabind::class_<HabitablePlanet, Planet>("HabitablePlanet")
            .def(luabind::constructor<>()),

            luabind::class_<WorldHelper>("WorldHelper")
            .def("objectByType", &WorldHelper::objectByType)
        ]
    ];
}

void rangersPluginDeinit()
{
}