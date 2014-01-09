from OpenSR.Engine import Engine, ResourceManager, ScriptWidget, ActionListener, Sprite, \
                          Rect, Action, NinePatch, ColorLabel, Button
from OpenSR.World import InteractionWidget, QuestPlayer

import OpenSR.ORC.StartMenu

engine = Engine.instance()
resources = ResourceManager.instance()
player = QuestPlayer()

class QuestWidget(ScriptWidget, ActionListener):
   
    def __init__(self):
        ScriptWidget.__init__(self)
        ActionListener.__init__(self)
        
        self.bg = Sprite("DATA/FormPQuest2/2BG.gi")
        self.addChild(self.bg)
        
        self.bg2 = Sprite("DATA/FormPQuest2/2S1.gi")
        self.bg2.position = (35, 5)
        self.bg2.layer = -1
        self.addChild(self.bg2)
        
        self.paramWindow = NinePatch("ORC/quest_param_win.9.json")
        self.paramWindow.position = (643, 457)
        self.paramWindow.geometry = (320, 280)
        self.addChild(self.paramWindow)
        
        self.paramLabel = ColorLabel("")
        self.paramLabel.wordWrap = True
        self.paramLabel.position = (665, 480)
        self.paramLabel.setFixedSize(320, 280)
        self.addChild(self.paramLabel)
        
        self.exitButton = Button("DATA/FormPQuest2/2CloseN.gi", "DATA/FormPQuest2/2CloseA.gi", "DATA/FormPQuest2/2CloseD.gi")
        self.exitButton.position = (953, 725)
        self.addWidget(self.exitButton)
        self.exitButton.addListener(self)
        
        player.loadQuest("CFG/Rus/PlanetQuest/Prison.qm")
        
        self.interact = InteractionWidget(Rect(50, 50, 50 + 539, 50 + 363), Rect(50, 487, 50 + 539, 487 + 215))
        self.addWidget(self.interact)
        self.interact.addListener(self)
        
        self.updateWidget()
        
    def updateWidget(self):
        self.selectionsID = []
        text = player.currentText.replace("\r\n", "\n").replace("\\cS", "\c0000FF")
        
        if player.questCompleted:
            text = text + "\n\n\\c00FF00Quest completed!\\cR"
        elif player.questFailed:
            text = text + "\n\n\\cFF0000Quest failed!\\cR"
            if player.death:
                text = text + "\n\n\\cFF0000You're dead!\\cR"
        
        self.interact.text = text
        
        trans = player.visibleTransitions
        selections = []
        for t in trans:
            self.selectionsID.append(t[0])
            selections.append((t[1], t[2]))
            
        self.interact.selections = selections
        
        self.paramLabel.text = "\n".join(player.visibleParameters).replace("\\cS", "\cFFFF00")            
        
    def dispose(self):
        engine.removeWidget(self)
                
        self.interact.removeListener(self)
        self.exitButton.removeListener(self)
                
    def actionPerformed(self, action):
        if action.type >= Action.Type.USER and action.source == self.interact: 
            index = action.type - Action.Type.USER
            player.transit(self.selectionsID[index])
            self.updateWidget()
        if action.type == Action.Type.BUTTON_CLICKED and action.source == self.exitButton:
            self.dispose()
            engine.addWidget(OpenSR.ORC.StartMenu.StartMenuWidget())
        