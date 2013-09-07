from OpenSR.Engine import Engine, ResourceManager
from OpenSR.World import WorldManager
from OpenSR.ORC.StartMenu import StartMenuWidget

import gettext

_ = gettext.gettext
gettext.textdomain('OpenSR')

engine = Engine.instance()
resources = ResourceManager.instance()
world = WorldManager.instance()

engine.addWidget(StartMenuWidget())
