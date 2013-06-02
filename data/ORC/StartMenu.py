import OpenSR.Engine
import gc
import sys
import gc

engine = OpenSR.Engine.Engine.instance()
resources = OpenSR.Engine.ResourceManager.instance()

class StartMenuWidget(OpenSR.Engine.ScriptWidget, OpenSR.Engine.ActionListener):
    def __init__(self, parent):
        OpenSR.Engine.ScriptWidget.__init__(self, parent)
        OpenSR.Engine.ActionListener.__init__(self)
        
        self.background = OpenSR.Engine.Sprite("DATA/FormMain3/2bg.gi")
        self.background.setWidth(self.background.width() / (engine.screenHeight() / self.background.height()))
        self.background.setHeight(engine.screenHeight())
        self.background.setLayer(-3)
        self.addChild(self.background)
                
        self.newGameButton = OpenSR.Engine.Button("DATA/FormMain2/2ButNewN.gi", "DATA/FormMain2/2ButNewA.gi", "DATA/FormMain2/2ButNewD.gi", self)
        self.newGameButton.addListener(self)
        self.addWidget(self.newGameButton)
        
        self.testLineEdit = OpenSR.Engine.LineEditWidget(100, 15)
        self.addWidget(self.testLineEdit)
        self.testLineEdit.setPosition(0, 100)
        self.testLineEdit.setLayer(1)
        
        #self.offset = 0.0
        
    def actionPerformed(self, action):
        if action.type() == OpenSR.Engine.ActionType.BUTTON_CLICKED:
            print(sys.getrefcount(self))
            print(gc.get_referrers(self))
            
            engine.removeWidget(self)
            
            #self.removeWidget(self.newGameButton)
            #self.removeChild(self.background)
            
            self.newGameButton.removeListener(self)
            
            print(sys.getrefcount(self))
            print(gc.get_referrers(self))  
        
    def __del__(self):
        print("DELETED")
            
      
