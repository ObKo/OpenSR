import OpenSR.Engine

engine = OpenSR.Engine.Engine.instance()
resources = OpenSR.Engine.ResourceManager.instance()
        
class StartMenuWidget(OpenSR.Engine.ScriptWidget, OpenSR.Engine.ActionListener):
    def __init__(self, parent):
        OpenSR.Engine.ScriptWidget.__init__(self, parent)
        OpenSR.Engine.ActionListener.__init__(self)
        
        self.background = OpenSR.Engine.Sprite("DATA/FormMain3/2bg.gi", self)
        self.background.setWidth(self.background.width() / (engine.screenHeight() / self.background.height()))
        self.background.setHeight(engine.screenHeight())
        self.background.setLayer(-3)
        
    def actionPerformed(self, action):
        if action.type() == OpenSR.Engine.ActionType.BUTTON_CLICKED:
            print("Action")
      
