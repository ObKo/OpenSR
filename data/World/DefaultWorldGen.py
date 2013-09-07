from OpenSR.World import WorldManager, WorldGenHook, PlanetarySystem, DesertPlanet, \
                         HabitablePlanet, Point
import math

class DefaultWorldGen(WorldGenHook):        
    def generate(self):
        world = WorldManager.instance()
        races = world.raceManager()
        systemManager = world.systemManager()
        
        system = PlanetarySystem()
        planet = DesertPlanet()
        sPlanet = DesertPlanet()
        hPlanet = HabitablePlanet()
        
        system.setName("Solar")
        system.setSize(2000.0)
        system.setStyle("solar")
        system.setPosition(Point(0.0, 0.0))

        planet.setOrbit(600.0)
        planet.setRadius(50.0)
        planet.setName("Mercur")
        planet.setAngle(0.0)
        planet.setAngleSpeed(math.pi * 2.0 / 87.0)
        planet.setStyle("mercur")

        hPlanet.setOrbit(450.0)
        hPlanet.setRadius(75.0)
        hPlanet.setName("Earth")
        hPlanet.setAngle(1.0)
        hPlanet.setAngleSpeed(math.pi * 2.0 / 365.0)
        hPlanet.setInvader(0)
        hPlanet.setPopulation(1000000)
        hPlanet.setRace(races.race("human").id)
        hPlanet.setStyle("earth")

        sPlanet.setOrbit(800.0)
        sPlanet.setRadius(100.0)
        sPlanet.setName("Saturn")
        sPlanet.setAngle(math.pi)
        sPlanet.setAngleSpeed(math.pi * 2.0 / 100.0)
        sPlanet.setStyle("saturn")

        system.addObject(planet)
        system.addObject(hPlanet)
        system.addObject(sPlanet)
        
        systemManager.addSystem(system)
        
        world.addObject(system)
        world.addObject(planet)
        world.addObject(sPlanet)
        world.addObject(hPlanet)
        
        systemManager.setCurrentSystem(system)
