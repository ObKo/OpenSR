from OpenSR.Engine import ResourceManager
from OpenSR.World import WorldManager, WorldGenHook, Point
from OpenSR.ORC.Utils import loadStylesFromDAT
import math
import OpenSR.World.WorldGen as wg

world = WorldManager.instance()
dat = ResourceManager.instance().datRoot()

DAT_SOLAR_STAR_STYLE = '04'
DAT_SOLAR_BG = 'bg70'
DAT_SOLAR_NAME = '02'
SOLAR_SIZE = 7500


# Mercur, Venus, Earth, Mars, Jupiter, Saturn, Neptune
SOLAR_PLANET_STYLES = ['203', '206', '200', '202', '201', '205', '204']
SOLAR_PLANET_NAMES = ['Solar.0', 'Solar.1', 'Solar.2', 'Solar.3', 'Solar.4', 'Solar.5', 'Solar.6']
SOLAR_PLANET_ORBITS = [500, 800, 1200, 1600, 2000, 2400, 3000]
SOLAR_PLANET_PERIODS = [40.0, 64.0, 96.0, 128.0, 160.0, 192.0, 240.0]
SOLAR_PLANET_HABITABLE = [False, True, True, True, False, False, False]
SOLAR_PLANET_RACE = [None, 'People', 'People', 'People', None, None, None]
SOLAR_PLANET_POPULATION = [None, 10000, 10000, 10000, None, None, None]


class WorldGen(WorldGenHook):  

    def generateSolar(self, position):
        races = world.raceManager()
        
        system = wg.genPlanetarySystem(DAT_SOLAR_NAME, SOLAR_SIZE, DAT_SOLAR_STAR_STYLE, DAT_SOLAR_BG, position)
        
        i = 0
        for p in SOLAR_PLANET_NAMES:
            if SOLAR_PLANET_HABITABLE[i]:
                wg.genHabitablePlanet(system, SOLAR_PLANET_ORBITS[i], int(dat['Data']['SE']['Planet'][SOLAR_PLANET_STYLES[i]]['Radius'].value), \
                                      SOLAR_PLANET_NAMES[i], math.pi * 0.75, math.pi * 2.0 / SOLAR_PLANET_PERIODS[i], SOLAR_PLANET_STYLES[i], \
                                      SOLAR_PLANET_POPULATION[i], races.race(SOLAR_PLANET_RACE[i]))
            else:
                wg.genDesertPlanet(system, SOLAR_PLANET_ORBITS[i], int(dat['Data']['SE']['Planet'][SOLAR_PLANET_STYLES[i]]['Radius'].value), \
                                   SOLAR_PLANET_NAMES[i], 0.0, math.pi * 2.0 / SOLAR_PLANET_PERIODS[i], SOLAR_PLANET_STYLES[i])
            i = i + 1
        return system
        
    def generate(self):        
        loadStylesFromDAT()                      
        world.systemManager().currentSystem = self.generateSolar(Point(0.0, 0.0)) 
