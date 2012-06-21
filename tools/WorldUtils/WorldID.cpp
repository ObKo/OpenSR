#include <iostream>
#include <boost/concept_check.hpp>

#include "Artifact.h"
#include "CargoHook.h"
#include "DefenceGenerator.h"
#include "DesertPlanet.h"
#include "Droid.h"
#include "Engine.h"
#include "Equipment.h"
#include "Goods.h"
#include "HabitablePlanet.h"
#include "Hull.h"
#include "InteractionContext.h"
#include "Item.h"
#include "LandContext.h"
#include "Meteor.h"
#include "Micromodulus.h"
#include "Planet.h"
#include "Radar.h"
#include "Scanner.h"
#include "Ship.h"
#include "ShipContext.h"
#include "SolarSystem.h"
#include "SpaceBase.h"
#include "SpaceObject.h"
#include "SystemObject.h"
#include "Tank.h"
#include "Types.h"
#include "Weapon.h"
#include "WorldObject.h"

using namespace Rangers::World;

int main(int argc, char **argv)
{
    std::cout << "Artifact: " << "0x" << std::hex << (new Artifact())->type() << std::endl;
    std::cout << "CargoHook: " << "0x" << std::hex << (new CargoHook())->type() << std::endl;
    std::cout << "DefenceGenerator: " << "0x" << std::hex << (new DefenceGenerator())->type() << std::endl;
    std::cout << "DesertPlanet: " << "0x" << std::hex << (new DesertPlanet())->type() << std::endl;
    std::cout << "Droid: " << "0x" << std::hex << (new Droid())->type() << std::endl;
    std::cout << "Engine: " << "0x" << std::hex << (new Engine())->type() << std::endl;
    std::cout << "Equipment: " << "0x" << std::hex << (new Equipment())->type() << std::endl;
    std::cout << "Goods: " << "0x" << std::hex << (new Goods())->type() << std::endl;
    std::cout << "HabitablePlanet: " << "0x" << std::hex << (new HabitablePlanet())->type() << std::endl;
    std::cout << "Hull: " << "0x" << std::hex << (new Hull())->type() << std::endl;
    std::cout << "Item: " << "0x" << std::hex << (new Item())->type() << std::endl;
    std::cout << "Meteor: " << "0x" << std::hex << (new Meteor())->type() << std::endl;
    std::cout << "Micromodulus: " << "0x" << std::hex << (new Micromodulus())->type() << std::endl;
    std::cout << "Planet: " << "0x" << std::hex << (new Planet())->type() << std::endl;
    std::cout << "Radar: " << "0x" << std::hex << (new Radar())->type() << std::endl;
    std::cout << "Scanner: " << "0x" << std::hex << (new Scanner())->type() << std::endl;
    std::cout << "Ship: " << "0x" << std::hex << (new Ship())->type() << std::endl;
    std::cout << "SolarSystem: " << "0x" << std::hex << (new SolarSystem())->type() << std::endl;
    std::cout << "SpaceBase: " << "0x" << std::hex << (new SpaceBase())->type() << std::endl;
    std::cout << "SpaceObject: " << "0x" << std::hex << (new SpaceObject())->type() << std::endl;
    std::cout << "SystemObject: " << "0x" << std::hex << (new SystemObject())->type() << std::endl;
    std::cout << "Tank: " << "0x" << std::hex << (new Tank())->type() << std::endl;
    std::cout << "Weapon: " << "0x" << std::hex << (new Weapon())->type() << std::endl;
    std::cout << "WorldObject: " << "0x" << std::hex << (new WorldObject())->type() << std::endl;
}
