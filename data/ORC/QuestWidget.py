from OpenSR.Engine import Engine, ResourceManager, ScriptWidget, ActionListener, Sprite, \
                          Rect, Action, NinePatch, ColorLabel, Button
from OpenSR.World import InteractionWidget, QuestPlayer

import OpenSR.ORC.StartMenu

engine = Engine.instance()
resources = ResourceManager.instance()
player = QuestPlayer()

class QuestWidget(ScriptWidget, ActionListener):
    
    def __init__(self, questFile):
        ScriptWidget.__init__(self)
        ActionListener.__init__(self)
        
        self.bg = Sprite("DATA/FormPQuest2/2BG.gi")
        self.addChild(self.bg)
        
        self.paramWindow = NinePatch("ORC/quest_param_win.9.json")
        self.paramWindow.position = (643, 457)
        self.paramWindow.geometry = (320, 280)
        self.addChild(self.paramWindow)
                
        self.exitButton = Button("DATA/FormPQuest2/2CloseN.gi", "DATA/FormPQuest2/2CloseA.gi", "DATA/FormPQuest2/2CloseD.gi")
        self.exitButton.position = (953, 725)
        self.addWidget(self.exitButton)
        self.exitButton.addListener(self)
        
        player.loadQuest(questFile)
        
        self.style = resources.objectManager().getResourceObject('/world/skin/quest/grey')
        self.interact = InteractionWidget(Rect(15, 45, 15 + 539, 45 + 363), Rect(15, 482, 15 + 539, 482 + 215), self.style)
        self.interact.position = (35, 5)
        self.interact.layer = -1;
        self.addWidget(self.interact)
        self.interact.addListener(self)
        
        self.paramLabel = ColorLabel("")
        self.paramLabel.wordWrap = True
        self.paramLabel.position = (665, 480)
        self.paramLabel.setFixedSize(320, 280)
        self.paramLabel.textColor = self.style.color
        self.paramLabel.selectionColor = self.style.activeColor
        self.addChild(self.paramLabel)
        
        self.skin1Button = Button("DATA/FormPQuest2/2Skin1N.gi", "DATA/FormPQuest2/2Skin1A.gi", "DATA/FormPQuest2/2Skin1D.gi")
        self.skin1Button.position = (529, 742)
        self.addWidget(self.skin1Button)
        self.skin1Button.addListener(self)
        
        self.skin2Button = Button("DATA/FormPQuest2/2Skin2N.gi", "DATA/FormPQuest2/2Skin2A.gi", "DATA/FormPQuest2/2Skin2D.gi")
        self.skin2Button.position = (547, 742)
        self.addWidget(self.skin2Button)
        self.skin2Button.addListener(self)
        
        self.skin3Button = Button("DATA/FormPQuest2/2Skin3N.gi", "DATA/FormPQuest2/2Skin3A.gi", "DATA/FormPQuest2/2Skin3D.gi")
        self.skin3Button.position = (565, 742)
        self.addWidget(self.skin3Button)
        self.skin3Button.addListener(self)
        
        self.skin4Button = Button("DATA/FormPQuest2/2Skin4N.gi", "DATA/FormPQuest2/2Skin4A.gi", "DATA/FormPQuest2/2Skin4D.gi")
        self.skin4Button.position = (583, 742)
        self.addWidget(self.skin4Button)
        self.skin4Button.addListener(self)
        
        self.updateWidget()
        
    def updateStyle(self):
        oldInteract = self.interact
              
        self.interact = InteractionWidget(Rect(15, 45, 15 + 539, 45 + 363), Rect(15, 482, 15 + 539, 482 + 215), self.style)
        self.interact.position = (35, 5)
        self.interact.layer = -1;
        self.addWidget(self.interact)
        self.interact.addListener(self)
        
        self.updateWidget()
        
        if oldInteract:
            oldInteract.removeListener(self)
            self.removeWidget(oldInteract)
        
    def updateWidget(self):
        self.selectionsID = []
        text = player.currentText.replace("\r\n", "\n")
        
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
        
        self.paramLabel.text = "\n".join(player.visibleParameters)      
        
    def dispose(self):
        engine.removeWidget(self)
                
        self.skin1Button.removeListener(self)
        self.skin2Button.removeListener(self)
        self.skin3Button.removeListener(self)
        self.skin4Button.removeListener(self)
        self.interact.removeListener(self)
        self.exitButton.removeListener(self)
                
    def actionPerformed(self, action):
        if action.type >= Action.Type.USER and action.source == self.interact: 
            index = action.type - Action.Type.USER
            player.transit(self.selectionsID[index])
            self.updateWidget()
        if action.type == Action.Type.BUTTON_CLICKED:
            if action.source == self.exitButton:
                self.dispose()
                engine.addWidget(OpenSR.ORC.StartMenu.StartMenuWidget())
            if action.source == self.skin1Button:
                self.style = resources.objectManager().getResourceObject('/world/skin/quest/grey')
                self.updateStyle()
            if action.source == self.skin2Button:
                self.style = resources.objectManager().getResourceObject('/world/skin/quest/dark')
                self.updateStyle()
            if action.source == self.skin3Button:
                self.style = resources.objectManager().getResourceObject('/world/skin/quest/light')
                self.updateStyle()
            if action.source == self.skin4Button:
                self.style = resources.objectManager().getResourceObject('/world/skin/quest/green')
                self.updateStyle()
        