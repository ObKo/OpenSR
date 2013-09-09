from OpenSR.World import WorldManager, WorldGenHook, PlanetarySystem, DesertPlanet, \
                         HabitablePlanet, Point, LandContext
import math

world = WorldManager.instance()

class DefaultWorldGen(WorldGenHook):  
    def genDesertPlanet(self, orbit, radius, name, angle, angleSpeed, style):
        planet = DesertPlanet()
        planet.setOrbit(orbit)
        planet.setRadius(radius)
        planet.setName(name)
        planet.setAngle(angle)
        planet.setAngleSpeed(angleSpeed)
        planet.setStyle(style)
        
        world.addObject(planet)
        
        return planet
        
    def genHabitablePlanet(self, orbit, radius, name, angle, angleSpeed, style, population, race, invader = None, relation = None):
        planet = HabitablePlanet()
        planet.setOrbit(orbit)
        planet.setRadius(radius)
        planet.setName(name)
        planet.setAngle(angle)
        planet.setAngleSpeed(angleSpeed)
        planet.setStyle(style)
        if invader:
            planet.setInvader(invader)
        planet.setPopulation(population)
        
        context = LandContext()
        world.addObject(context)
        context.setRace(race.id)
        if relation:
            planet.landContext().setRelation(relation)
    
        planet.setLandContext(context)
        
        world.addObject(planet)
        
        return planet
      
    def generate(self):
        races = world.raceManager()
        systemManager = world.systemManager()
        
        system = PlanetarySystem()
        
        system.setName("Solar")
        system.setSize(2000.0)
        system.setStyle("solar")
        system.setPosition(Point(0.0, 0.0))
        
        planet = self.genDesertPlanet(600.0, 50.0, "Mercur", 0.0, math.pi * 2.0 / 87.0, "mercur")
        hPlanet = self.genHabitablePlanet(450.0, 75.0, "Earth", 1.0, math.pi * 2.0 / 365.0, "earth", 1000000, races.race("human"))
        sPlanet = self.genDesertPlanet(800.0, 100.0, "Saturn", math.pi, math.pi * 2.0 / 100.0, "saturn")

        system.addObject(planet)
        system.addObject(hPlanet)
        system.addObject(sPlanet)
        
        systemManager.addSystem(system)
        world.addObject(system)
        
        systemManager.setCurrentSystem(system)
