import OpenSR.Engine
import OpenSR.ORC.StartMenu
import OpenSR.World
import gettext

_ = gettext.gettext
gettext.textdomain('OpenSR')

engine = OpenSR.Engine.Engine.instance()
resources = OpenSR.Engine.ResourceManager.instance()
world = OpenSR.World.WorldManager.instance()

engine.addWidget(OpenSR.ORC.StartMenu.StartMenuWidget())
