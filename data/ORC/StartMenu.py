from OpenSR.World import WorldManager, DefaultWorldGen, SystemWidget
from OpenSR.Engine import Engine, ResourceManager, SoundManager, ScriptWidget, ActionListener, \
                          Sprite, GAISprite, SpriteXOrigin, SpriteYOrigin, WidgetNode, Button, \
                          ActionType, LineEdit
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
        self.background.setWidth(self.background.width() / (engine.screenHeight() / self.background.height()))
        self.background.setHeight(engine.screenHeight())
        self.background.setLayer(-3)
        self.addChild(self.background)
        
        self.bgPlanet = Sprite('DATA/FormMain3/2Planet.gi')
        self.bgPlanet.setOrigin(SpriteXOrigin.POSITION_X_LEFT, SpriteYOrigin.POSITION_Y_BOTTOM)
        self.bgPlanet.setPosition(0, engine.screenHeight())
        self.bgPlanet.setLayer(-2)
        self.addChild(self.bgPlanet)

        self.bgAnimLine = GAISprite("DATA/FormMain2/2AnimLine.gai")
        self.bgAnimLine.setOrigin(SpriteXOrigin.POSITION_X_RIGHT, SpriteYOrigin.POSITION_Y_TOP)
        self.bgAnimLine.setPosition(engine.screenWidth(), 0)
        self.bgAnimLine.setLayer(0)
        self.addChild(self.bgAnimLine)

        self.bgAnim = GAISprite("DATA/FormMain2/2AnimMain.gai")
        self.bgAnim.setLayer(-1)
        self.bgAnim.setHeight(self.bgAnim.height() * engine.screenWidth() / self.bgAnim.width())
        self.bgAnim.setWidth(engine.screenWidth())
        self.bgAnim.setOrigin(SpriteXOrigin.POSITION_X_LEFT, SpriteYOrigin.POSITION_Y_BOTTOM)
        self.bgAnim.setPosition(0, engine.screenHeight())
        self.addChild(self.bgAnim)

        self.caption = Sprite("DATA/FormMain3/2caption.gi")
        self.caption.setOrigin(SpriteXOrigin.POSITION_X_RIGHT, SpriteYOrigin.POSITION_Y_CENTER)
        self.caption.setPosition(engine.screenWidth(), 110)
        self.caption.setLayer(1)
        self.addChild(self.caption)
        
        self.ship = GAISprite('DATA/FormMain3/2Ship1.gai')
        self.ship.setPosition(0, engine.screenHeight() - self.ship.height())
        self.ship.setLayer(1)
        self.addChild(self.ship)
        
        self.menuNode = WidgetNode()
        self.menuNode.setLayer(2)
        self.menuNode.setPosition(engine.screenWidth() - 400, 300)
        self.addWidget(self.menuNode)
        
        self.edit = LineEdit(500)
        self.edit.setPosition(0, -100)
        self.edit.addListener(self)
        self.menuNode.addWidget(self.edit)
        
        sound.playMusic("music/SPECIAL/SpaceIsCalling.dat", False)
        
        self.buttons = {}
        
        clickSound = sound.loadSound("Sound/ButtonClick.wav")
        leaveSound = sound.loadSound("Sound/ButtonLeave.WAV")
        enterSound = sound.loadSound("Sound/ButtonEnter.wav")
        
        y = 0
        for name in self.BUTTONS:  
            button = Button('DATA/FormMain2/' + self.BUTTON_FILES[name] + 'N.gi', 'DATA/FormMain2/' + self.BUTTON_FILES[name] + 'A.gi', 'DATA/FormMain2/' + self.BUTTON_FILES[name] + 'D.gi')
            self.buttons[name] = button
            button.addListener(self)
            button.setPosition(0, y)
            button.clickSound = clickSound
            button.enterSound = enterSound
            button.leaveSound = leaveSound
            self.menuNode.addWidget(button)
            y = y + self.BUTTON_SPARSE

    def processLogic(self, dt):
        ScriptWidget.processLogic(self, dt)
        
        if math.fabs(self.t * self.BG_SPEED) >= self.background.width() - engine.screenWidth():
            self.t = 0
            self.bgforward = not self.bgforward
        if self.bgforward:
            self.background.setPosition(- self.t * self.BG_SPEED, 0)
        else:
            self.background.setPosition(- self.background.width() + engine.screenWidth() +  self.t * self.BG_SPEED, 0)
 
        self.t = self.t + dt
        
    def dispose(self):
        engine.removeWidget(self)
        
        for name in self.BUTTONS:
            self.buttons[name].removeListener(self) 
            
    def openGame(self):
        widget = SystemWidget(world.systemManager().currentSystem())   
        engine.addWidget(widget)
        self.dispose()
        
    def actionPerformed(self, action):        
        if action.type != ActionType.BUTTON_CLICKED:
            return

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