import OpenSR.Engine
import OpenSR.ORC.StartMenu
import sys
import gc
import gettext

_ = gettext.gettext
gettext.textdomain('OpenSR')

gc.set_debug(gc.DEBUG_COLLECTABLE)

engine = OpenSR.Engine.Engine.instance()
resources = OpenSR.Engine.ResourceManager.instance()

w = OpenSR.ORC.StartMenu.StartMenuWidget(None)
engine.addWidget(w)
