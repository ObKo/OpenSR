from OpenSR.World import WorldManager, WorldGenHook, Point
#from OpenSR.ORC.Utils import loadPlanetStylesFromDAT, loadSystemStylesFromDAT, loadAsteroidStylesFromDAT
import math
import OpenSR.World.WorldGen as WorldGen

world = WorldManager.instance()

class DefaultWorldGen(WorldGenHook):  
    def generate(self):        
        races = world.raceManager()
       
        system = WorldGen.genPlanetarySystem('Solar', 2000.0, 'solar', 'bg00', Point(0.0, 0.0))
        
        WorldGen.genDesertPlanet(system, 600.0, 50.0, 'Mercur', 0.0, math.pi * 2.0 / 87.0, 'mercur')
        WorldGen.genHabitablePlanet(system, 450.0, 75.0, 'Earth', 1.0, math.pi * 2.0 / 365.0, 'earth', 1000000, races.race('People'))
        WorldGen.genDesertPlanet(system, 800.0, 100.0, 'Saturn', math.pi, math.pi * 2.0 / 100.0, 'saturn')
        WorldGen.genAsteroid(system, 1000.0, 500.0, 20.0, 0.0, 0.0, 100.0, '00')   
        WorldGen.genAsteroid(system, 1500.0, 700.0, 10.0, 100.0, 0.0, 100.0, '01')
                       
        world.systemManager().currentSystem = system
