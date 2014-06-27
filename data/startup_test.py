from OpenSR.Engine import Engine, ResourceManager
from OpenSR.Engine import tr as _
from OpenSR.test.TestingWidget import TestingWidget

engine = Engine.instance()
resources = ResourceManager.instance()

engine.loadPlugin('world')

resources.addDATFile('CacheData.dat', True)
resources.addDATFile('Main.dat')
resources.addDATFile('Rus.dat')

engine.addWidget(TestingWidget())
