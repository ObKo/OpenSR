from OpenSR.World import WorldManager, PlanetarySystem, DesertPlanet, HabitablePlanet, LandContext, Asteroid
from OpenSR.Engine import textHash32

world = WorldManager.instance()
  
def genDesertPlanet(system, orbit, radius, name, angle, angleSpeed, style):
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
    
def genHabitablePlanet(system, orbit, radius, name, angle, angleSpeed, style, population, race, invader = None, relation = None):
    planet = HabitablePlanet()
    planet.orbit = orbit
    planet.radius = radius
    planet.name = name
    planet.angle = angle
    planet.angleSpeed = angleSpeed
    planet.style = style
    if invader:
        planet.invader = invader
    planet.population = population
    
    context = LandContext()
    world.addObject(context)
    context.race = textHash32(race.id)
    if relation:
        planet.landContext().relation = relation

    planet.landContext = context
    
    world.addObject(planet)
    system.addObject(planet)
    
    return planet
  
def genAsteroid(system, a, b, period, angle, time, mineral, style):
    asteroid = Asteroid()
    asteroid.semiAxis = (a, b)
    asteroid.period = period
    asteroid.angle = angle
    asteroid.time = time
    asteroid.mineral = mineral
    asteroid.style = style
    
    world.addObject(asteroid)
    system.addObject(asteroid)
    
  
def genPlanetarySystem(name, size, style, background, position):
    system = PlanetarySystem()
    
    system.name = name
    system.size = size
    system.style = style
    system.position = position
    system.background = background
    
    world.systemManager().addSystem(system)
    world.addObject(system)
    
    return system 
