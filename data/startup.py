import OpenSR.Engine
import sys
import gc

gc.set_debug(gc.DEBUG_COLLECTABLE)
        
class TestWidget(OpenSR.Engine.ScriptWidget):
    def __init__(self, parent):
        OpenSR.Engine.ScriptWidget.__init__(self, parent)
        self.s = OpenSR.Engine.Sprite("DATA/Items/2ArtDef.gi", self)
        self.text = OpenSR.Engine.Label("Test", self)
        self.text.setColor(0x000FFFF)
        
    def __del__(self):
        print("FUCK!")
        sys.stdout.flush()
    
    def processLogic(self, ms):
        pos = self.s.position()
        pos.x = pos.x + ms / 10.0
        pos.y = pos.y + ms / 10.0
        self.s.setPosition(pos)
        self.text.setPosition(pos)
        
engine = OpenSR.Engine.Engine.instance()

w = TestWidget(None)
engine.addWidget(w)
