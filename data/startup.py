import OpenSR.Engine
import OpenSR.ORC.StartMenu
import gettext

_ = gettext.gettext
gettext.textdomain('OpenSR')

engine = OpenSR.Engine.Engine.instance()
resources = OpenSR.Engine.ResourceManager.instance()

engine.addWidget(OpenSR.ORC.StartMenu.StartMenuWidget(None))
