from OpenSR.World import WorldManager, WorldGenHook, PlanetarySystem, DesertPlanet, \
                         HabitablePlanet, Point, LandContext, Asteroid
import math

world = WorldManager.instance()
systemManager = world.systemManager()

class DefaultWorldGen(WorldGenHook):  
    def genDesertPlanet(self, system, orbit, radius, name, angle, angleSpeed, style):
        planet = DesertPlanet()
        planet.orbit = orbit
        planet.radius = radius
        planet.name = name
        planet.angle = angle
        planet.angleSpeed = angleSpeed
        planet.style = style
        
        world.addObject(planet)
        system.addObject(planet)
        
        return planet
        
    def genHabitablePlanet(self, system, orbit, radius, name, angle, angleSpeed, style, population, race, invader = None, relation = None):
        planet = HabitablePlanet()
        planet.orbit = orbit
        planet.radius = radius
        planet.name = name
        planet.angle = angle
        planet.angleSpeed = angleSpeed
        planet.style = style
        if invader:
            planet.invader = invader
        planet.opulation = population
        
        context = LandContext()
        world.addObject(context)
        context.race = race.id
        if relation:
            planet.landContext().relation = relation
    
        planet.landContext = context
        
        world.addObject(planet)
        system.addObject(planet)
        
        return planet
      
    def genAsteroid(self, system, a, b, period, angle, time, mineral, style):
        asteroid = Asteroid()
        asteroid.semiAxis = (a, b)
        asteroid.period = period
        asteroid.angle = angle
        asteroid.time = time
        asteroid.mineral = mineral
        asteroid.style = style
        
        world.addObject(asteroid)
        system.addObject(asteroid)
        
      
    def genPlanetarySystem(self, name, size, style, position):
        system = PlanetarySystem()
        
        system.name = name
        system.size = size
        system.style = style
        system.position = position
        
        systemManager.addSystem(system)
        world.addObject(system)
        
        return system
      
    def generate(self):
        races = world.raceManager()
       
        system = self.genPlanetarySystem('Solar', 2000.0, 'solar', Point(0.0, 0.0))
        
        self.genDesertPlanet(system, 600.0, 50.0, 'Mercur', 0.0, math.pi * 2.0 / 87.0, 'mercur')
        self.genHabitablePlanet(system, 450.0, 75.0, 'Earth', 1.0, math.pi * 2.0 / 365.0, 'earth', 1000000, races.race('human'))
        self.genDesertPlanet(system, 800.0, 100.0, 'Saturn', math.pi, math.pi * 2.0 / 100.0, 'saturn')
        self.genAsteroid(system, 1000.0, 500.0, 20.0, 0.0, 0.0, 100.0, '00')   
        self.genAsteroid(system, 1500.0, 700.0, 10.0, 100.0, 0.0, 100.0, '01')
                
        systemManager.currentSystem = system
