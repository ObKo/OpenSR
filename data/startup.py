from OpenSR.Engine import Engine, ResourceManager

import gettext

_ = gettext.gettext
gettext.textdomain('OpenSR')

engine = Engine.instance()
resources = ResourceManager.instance()

engine.loadPlugin('world')

from OpenSR.ORC.StartMenu import StartMenuWidget
engine.addWidget(StartMenuWidget())
