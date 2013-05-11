import OpenSR.Engine
import sys
import gc
import gettext

_ = gettext.gettext
gettext.textdomain('OpenSR')

gc.set_debug(gc.DEBUG_COLLECTABLE)
        
class TestWidget(OpenSR.Engine.ScriptWidget, OpenSR.Engine.ActionListener):
    def __init__(self, parent):
        OpenSR.Engine.ScriptWidget.__init__(self, parent)
        OpenSR.Engine.ActionListener.__init__(self)
        self.newGameButton = OpenSR.Engine.Button("DATA/FormMain2/2ButNewN.gi", "DATA/FormMain2/2ButNewA.gi", "DATA/FormMain2/2ButNewD.gi", self)
        self.newGameButton.addListener(self)
        
    def actionPerformed(self, action):
        if action.type() == OpenSR.Engine.ActionType.BUTTON_CLICKED:
            print("Action")
        
    def __del__(self):
        print("Delete")
        sys.stdout.flush()
        
engine = OpenSR.Engine.Engine.instance()
resources = OpenSR.Engine.ResourceManager.instance()

w = TestWidget(None)
engine.addWidget(w)
