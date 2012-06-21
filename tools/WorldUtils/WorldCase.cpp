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
    std::cout << std::hex << "case 0x" << (new Artifact())->type() << ":\n\tobject = new Artifact();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new CargoHook())->type() << ":\n\tobject = new CargoHook();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new DefenceGenerator())->type() << ":\n\tobject = new DefenceGenerator();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new DesertPlanet())->type() << ":\n\tobject = new DesertPlanet();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Droid())->type() << ":\n\tobject = new Droid();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Engine())->type() << ":\n\tobject = new Engine();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Equipment())->type() << ":\n\tobject = new Equipment();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Goods())->type() << ":\n\tobject = new Goods();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new HabitablePlanet())->type() << ":\n\tobject = new HabitablePlanet();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Hull())->type() << ":\n\tobject = new Hull();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Item())->type() << ":\n\tobject = new Item();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Meteor())->type() << ":\n\tobject = new Meteor();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Micromodulus())->type() << ":\n\tobject = new Micromodulus();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Planet())->type() << ":\n\tobject = new Planet();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Radar())->type() << ":\n\tobject = new Radar();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Scanner())->type() << ":\n\tobject = new Scanner();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Ship())->type() << ":\n\tobject = new Ship();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new SolarSystem())->type() << ":\n\tobject = new SolarSystem();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new SpaceBase())->type() << ":\n\tobject = new SpaceBase();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new SpaceObject())->type() << ":\n\tobject = new SpaceObject();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new SystemObject())->type() << ":\n\tobject = new SystemObject();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Tank())->type() << ":\n\tobject = new Tank();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new Weapon())->type() << ":\n\tobject = new Weapon();\n\tbreak;\n";
    std::cout << std::hex << "case 0x" << (new WorldObject())->type() << ":\n\tobject = new WorldObject();\n\tbreak;\n";
}
