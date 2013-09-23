#include <iostream>
#include <boost/concept_check.hpp>

#include "Artifact.h"
#include "Asteroid.h"
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
#include "Micromodulus.h"
#include "Planet.h"
#include "Radar.h"
#include "Scanner.h"
#include "Ship.h"
#include "ShipContext.h"
#include "PlanetarySystem.h"
#include "SpaceBase.h"
#include "SpaceObject.h"
#include "SystemObject.h"
#include "Tank.h"
#include "Types.h"
#include "Weapon.h"
#include "WorldObject.h"
#include <list>
#include <string>

std::list<std::string> classes;

using namespace Rangers::World;

int main(int argc, char **argv)
{
    classes.push_back("Artifact");
    classes.push_back("Asteroid");
    classes.push_back("CargoHook");
    classes.push_back("DefenceGenerator");
    classes.push_back("DesertPlanet");
    classes.push_back("Droid");
    classes.push_back("Engine");
    classes.push_back("Equipment");
    classes.push_back("Goods");
    classes.push_back("HabitablePlanet");
    classes.push_back("Hull");
    classes.push_back("Item");
    classes.push_back("Micromodulus");
    classes.push_back("Planet");
    classes.push_back("Radar");
    classes.push_back("Scanner");
    classes.push_back("Ship");
    classes.push_back("PlanetarySystem");
    classes.push_back("SpaceBase");
    classes.push_back("SpaceObject");
    classes.push_back("SystemObject");
    classes.push_back("Tank");
    classes.push_back("Weapon");
    classes.push_back("WorldObject");
    for (std::list<std::string>::iterator i = classes.begin(); i != classes.end(); ++i)
    {
        std::string upper(*i);
        std::transform((*i).begin(), (*i).end(), upper.begin(), toupper);
        std::cout << "case TYPE_" + upper + ":\n\tobject = new " + (*i) + "();\n\tbreak;\n";
    }
}
