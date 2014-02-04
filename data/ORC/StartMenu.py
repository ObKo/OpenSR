from OpenSR.World import WorldManager, DefaultWorldGen, SystemWidget
from OpenSR.Engine import Engine, ResourceManager, SoundManager, ScriptWidget, ActionListener, \
                          Sprite, GAISprite, SpriteXOrigin, SpriteYOrigin, WidgetNode, Button, \
                          Action, Label, LabelWidget, ColorLabel

from OpenSR.ORC.QuestWidget import QuestWidget
from OpenSR.ORC.QuestSelectionWidget import QuestSelectionWidget

import OpenSR.ORC.Settings
from OpenSR.World.DefaultWorldGen import DefaultWorldGen
import sys
import math

engine = Engine.instance()
resources = ResourceManager.instance()
sound = SoundManager.instance()
world = WorldManager.instance()

class StartMenuWidget(ScriptWidget, ActionListener):
    BG_SPEED = 20.0 / 1000.0
    BUTTON_FILES = {'newGame': '2ButNew', 'loadGame': '2ButLoad', 'settings': '2ButSettings', 'records': '2ButRecords', 'about': '2ButAbout', 'exit': '2ButExit'}
    BUTTONS = ['newGame', 'loadGame', 'settings', 'records', 'about', 'exit']
    BUTTON_SPARSE = 60
    
    def __init__(self):
        ScriptWidget.__init__(self)
        ActionListener.__init__(self)
                
        self.t = 0
        self.bgforward = False
        
        self.background = Sprite('DATA/FormMain3/2bg.gi')
        self.background.width = self.background.width / (engine.screenHeight / self.background.height)
        self.background.height = engine.screenHeight
        self.background.layer = -3
        self.addChild(self.background)
        
        self.bgPlanet = Sprite('DATA/FormMain3/2Planet.gi')
        self.bgPlanet.origin = (SpriteXOrigin.POSITION_X_LEFT, SpriteYOrigin.POSITION_Y_BOTTOM)
        self.bgPlanet.position = (0, engine.screenHeight)
        self.bgPlanet.layer = -2
        self.addChild(self.bgPlanet)

        self.bgAnimLine = GAISprite("DATA/FormMain2/2AnimLine.gai")
        self.bgAnimLine.origin = (SpriteXOrigin.POSITION_X_RIGHT, SpriteYOrigin.POSITION_Y_TOP)
        self.bgAnimLine.position = (engine.screenWidth, 0)
        self.bgAnimLine.layer = 0
        self.addChild(self.bgAnimLine)

        self.bgAnim = GAISprite("DATA/FormMain2/2AnimMain.gai")
        self.bgAnim.layer = -1
        self.bgAnim.height = self.bgAnim.height * engine.screenWidth / self.bgAnim.width
        self.bgAnim.width = engine.screenWidth
        self.bgAnim.origin = (SpriteXOrigin.POSITION_X_LEFT, SpriteYOrigin.POSITION_Y_BOTTOM)
        self.bgAnim.position = (0, engine.screenHeight)
        self.addChild(self.bgAnim)

        self.caption = Sprite("DATA/FormMain3/2caption.gi")
        self.caption.origin = (SpriteXOrigin.POSITION_X_RIGHT, SpriteYOrigin.POSITION_Y_CENTER)
        self.caption.position = (engine.screenWidth, 110)
        self.caption.layer = 1
        self.addChild(self.caption)
        
        self.ship = [None, None, None]
        self.ship[0] = GAISprite('DATA/FormMain3/2Ship1.gai')
        self.ship[0].position = (0, engine.screenHeight - self.ship[0].height)
        self.ship[0].layer = 1
        self.ship[0].speed = 1.5
        self.ship[0].singleShot = True
        
        self.ship[1] = GAISprite('DATA/FormMain3/2Ship2.gai')
        self.ship[1].position = (0, engine.screenHeight - self.ship[1].height)
        self.ship[1].layer = 1
        self.ship[1].speed = 1.5
        self.ship[1].singleShot = True
        self.ship[1].stop();
        
        self.ship[2] = GAISprite('DATA/FormMain3/2Ship3.gai')
        self.ship[2].position = (0, engine.screenHeight - self.ship[2].height)
        self.ship[2].layer = 1
        self.ship[2].speed = 1.5
        self.ship[2].singleShot = True
        self.ship[2].stop();
        
        self.addChild(self.ship[0])
        self.currentShip = 0
        
        self.menuNode = WidgetNode()
        self.menuNode.layer = 2
        self.menuNode.position = (engine.screenWidth - 400, 300)
        self.addWidget(self.menuNode)
        
        sound.playMusic("Music/SPECIAL/SpaceIsCalling.dat", False)
        
        self.buttons = {}
        
        clickSound = "Sound/ButtonClick.wav"
        leaveSound = "Sound/ButtonLeave.WAV"
        enterSound = "Sound/ButtonEnter.wav"
        
        y = 0
        for name in self.BUTTONS:  
            button = Button('DATA/FormMain2/' + self.BUTTON_FILES[name] + 'N.gi', 'DATA/FormMain2/' + self.BUTTON_FILES[name] + 'A.gi', 'DATA/FormMain2/' + self.BUTTON_FILES[name] + 'D.gi')
            self.buttons[name] = button
            button.addListener(self)
            button.position = (0, y)
            button.sounds = (clickSound, enterSound, leaveSound)
            self.menuNode.addWidget(button)
            y = y + self.BUTTON_SPARSE

        self.questButton = Button('DATA/FormLoadRobot/2LoadQuestN.gi', 'DATA/FormLoadRobot/2LoadQuestA.gi', 'DATA/FormLoadRobot/2LoadQuestN.gi')
        self.questButton.addListener(self)
        self.questButton.position = (305, engine.screenHeight - 135)
        self.questButton.layer = 2
        self.questButton.sounds = (clickSound, enterSound, leaveSound)
        self.addWidget(self.questButton)
        
        self.pause = False

    def processLogic(self, dt):
        if self.pause:
            return

        ScriptWidget.processLogic(self, dt)
        
        if math.fabs(self.t * self.BG_SPEED) >= self.background.width - engine.screenWidth:
            self.t = 0
            self.bgforward = not self.bgforward
        if self.bgforward:
            self.background.position = (- self.t * self.BG_SPEED, 0)
        else:
            self.background.position = (- self.background.width + engine.screenWidth +  self.t * self.BG_SPEED, 0)
 
        if not self.ship[self.currentShip].started:
            self.removeChild(self.ship[self.currentShip])
            self.currentShip = (self.currentShip + 1) % 3
            self.addChild(self.ship[self.currentShip])
            self.ship[self.currentShip].reset()
            self.ship[self.currentShip].start()
 
        self.t = self.t + dt
        
    def dispose(self):
        engine.removeWidget(self)
        
        for name in self.BUTTONS:
            self.buttons[name].removeListener(self) 
        
        self.questButton.removeListener(self)
            
    def openGame(self):
        widget = SystemWidget(world.systemManager().currentSystem)   
        engine.addWidget(widget)
        self.dispose()
        
    def startQuest(self, questFile):
        widget = QuestWidget(questFile)   
        engine.addWidget(widget)
        self.dispose()
        
    def actionPerformed(self, action):        
        if action.type == Action.Type.BUTTON_CLICKED:
            if action.source == self.buttons['exit']:
                self.dispose()                
                engine.quit(0)
                
            elif action.source == self.buttons['settings']:
                self.dispose()
                engine.addWidget(OpenSR.ORC.Settings.SettingsWidget())
                
            elif action.source == self.buttons['newGame']:
                world.addGenHook(DefaultWorldGen())
                world.generateWorld()
                world.saveWorld('test.srw')
                self.openGame()
                
            elif action.source == self.buttons['loadGame']:
                world.loadWorld('test.srw')
                self.openGame()   
                
            elif action.source == self.questButton:
                self.pause = True
                self.questDialog = OpenSR.ORC.QuestSelectionWidget.QuestSelectionWidget()
                self.questDialog.layer = 4
                self.questDialog.position = (int((engine.screenWidth - self.questDialog.width) / 2), int((engine.screenHeight - self.questDialog.height) / 2))
                self.questDialog.addListener(self)
                engine.addWidget(self.questDialog)
                
        if action.type == Action.Type.DIALOG_ACCEPT:
            if action.source == self.questDialog:
                self.pause = False
                
                self.questDialog.dispose()
                self.questDialog.removeListener(self)
                engine.removeWidget(self.questDialog)
                
                qmFile = self.questDialog.selectedQuest
                
                del self.questDialog
                
                self.startQuest(qmFile)
                
        elif action.type == Action.Type.DIALOG_CANCEL:
            if action.source == self.questDialog:
                self.pause = False
                
                self.questDialog.dispose()
                self.questDialog.removeListener(self)
                engine.removeWidget(self.questDialog)
                del self.questDialog
        
