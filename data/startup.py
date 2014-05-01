from OpenSR.Engine import Engine, ResourceManager
from OpenSR.Engine import tr as _

engine = Engine.instance()
resources = ResourceManager.instance()

engine.loadPlugin('world')

from OpenSR.ORC.StartMenu import StartMenuWidget
engine.addWidget(StartMenuWidget())

resources.addDATFile('CacheData.dat', True)
resources.addDATFile('Main.dat')
resources.addDATFile('Rus.dat')
