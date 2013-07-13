import OpenSR.World
import OpenSR.Engine
import OpenSR.ORC.Settings
import OpenSR.World.DefaultWorldGen
import sys
import math

engine = OpenSR.Engine.Engine.instance()
resources = OpenSR.Engine.ResourceManager.instance()
sound = OpenSR.Engine.SoundManager.instance()

class StartMenuWidget(OpenSR.Engine.ScriptWidget, OpenSR.Engine.ActionListener):
    BG_SPEED = 20.0 / 1000.0
    BUTTON_FILES = {'newGame': '2ButNew', 'loadGame': '2ButLoad', 'settings': '2ButSettings', 'records': '2ButRecords', 'about': '2ButAbout', 'exit': '2ButExit'}
    BUTTONS = ['newGame', 'loadGame', 'settings', 'records', 'about', 'exit']
    BUTTON_SPARSE = 60
    
    def __init__(self):
        OpenSR.Engine.ScriptWidget.__init__(self)
        OpenSR.Engine.ActionListener.__init__(self)
                
        self.t = 0
        self.bgforward = False
        
        self.background = OpenSR.Engine.Sprite('DATA/FormMain3/2bg.gi')
        self.background.setWidth(self.background.width() / (engine.screenHeight() / self.background.height()))
        self.background.setHeight(engine.screenHeight())
        self.background.setLayer(-3)
        self.addChild(self.background)
        
        self.bgPlanet = OpenSR.Engine.Sprite('DATA/FormMain3/2Planet.gi')
        self.bgPlanet.setOrigin(OpenSR.Engine.SpriteXOrigin.POSITION_X_LEFT, OpenSR.Engine.SpriteYOrigin.POSITION_Y_BOTTOM)
        self.bgPlanet.setPosition(0, engine.screenHeight())
        self.bgPlanet.setLayer(-2)
        self.addChild(self.bgPlanet)

        self.bgAnimLine = OpenSR.Engine.GAISprite("DATA/FormMain2/2AnimLine.gai")
        self.bgAnimLine.setOrigin(OpenSR.Engine.SpriteXOrigin.POSITION_X_RIGHT, OpenSR.Engine.SpriteYOrigin.POSITION_Y_TOP)
        self.bgAnimLine.setPosition(engine.screenWidth(), 0)
        self.bgAnimLine.setLayer(0)
        self.addChild(self.bgAnimLine)

        self.bgAnim = OpenSR.Engine.GAISprite("DATA/FormMain2/2AnimMain.gai")
        self.bgAnim.setLayer(-1)
        self.bgAnim.setHeight(self.bgAnim.height() * engine.screenWidth() / self.bgAnim.width())
        self.bgAnim.setWidth(engine.screenWidth())
        self.bgAnim.setOrigin(OpenSR.Engine.SpriteXOrigin.POSITION_X_LEFT, OpenSR.Engine.SpriteYOrigin.POSITION_Y_BOTTOM)
        self.bgAnim.setPosition(0, engine.screenHeight())
        self.addChild(self.bgAnim)

        self.caption = OpenSR.Engine.Sprite("DATA/FormMain3/2caption.gi")
        self.caption.setOrigin(OpenSR.Engine.SpriteXOrigin.POSITION_X_RIGHT, OpenSR.Engine.SpriteYOrigin.POSITION_Y_CENTER)
        self.caption.setPosition(engine.screenWidth(), 110)
        self.caption.setLayer(1)
        self.addChild(self.caption)
        
        self.ship = OpenSR.Engine.GAISprite('DATA/FormMain3/2Ship1.gai')
        self.ship.setPosition(0, engine.screenHeight() - self.ship.height())
        self.ship.setLayer(1)
        self.addChild(self.ship)
        
        self.menuNode = OpenSR.Engine.WidgetNode()
        self.menuNode.setLayer(2)
        self.menuNode.setPosition(engine.screenWidth() - 400, 300)
        self.addWidget(self.menuNode)
        
        sound.playMusic("music/SPECIAL/SpaceIsCalling.dat", False)
        
        self.buttons = {}
        
        y = 0
        for name in self.BUTTONS:  
            self.buttons[name] = OpenSR.Engine.Button('DATA/FormMain2/' + self.BUTTON_FILES[name] + 'N.gi', 'DATA/FormMain2/' + self.BUTTON_FILES[name] + 'A.gi', 'DATA/FormMain2/' + self.BUTTON_FILES[name] + 'D.gi')
            self.buttons[name].addListener(self)
            self.buttons[name].setPosition(0, y)
            self.buttons[name].setSounds("Sound/ButtonClick.wav", "Sound/ButtonLeave.WAV", "Sound/ButtonEnter.wav")
            self.menuNode.addWidget(self.buttons[name])
            y = y + self.BUTTON_SPARSE

    def processLogic(self, dt):
        OpenSR.Engine.ScriptWidget.processLogic(self, dt)
        
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
        
    def actionPerformed(self, action):
        if action.type() != OpenSR.Engine.ActionType.BUTTON_CLICKED:
            return

        if action.source() == self.buttons['exit']:
            self.dispose()                
            engine.quit(0)
        elif action.source() == self.buttons['settings']:
            self.dispose()
            engine.addWidget(OpenSR.ORC.Settings.SettingsWidget())
        elif action.source() == self.buttons['newGame']:
            world = OpenSR.World.WorldManager.instance()
            world.addGenHook(OpenSR.World.DefaultWorldGen.DefaultWorldGen())
            world.generateWorld()
            
            widget = OpenSR.World.SystemWidget(world.systemManager().currentSystem())
            
            engine.addWidget(widget)
            self.dispose();
            