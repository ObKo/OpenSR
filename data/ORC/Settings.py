from OpenSR.Engine import Engine, ResourceManager, SoundManager, ScriptWidget, ActionListener, \
                          Sprite, NinePatch, WidgetNode, Button, Label, LineEdit, CheckBox, \
                          ScrollArea, ActionType
import OpenSR.ORC.StartMenu
from gettext import gettext

import sys
import math

_ = gettext

engine = Engine.instance()
resources = ResourceManager.instance()
sound = SoundManager.instance()

class SettingsWidget(ScriptWidget, ActionListener):
   
    def __init__(self):
        ScriptWidget.__init__(self)
        ActionListener.__init__(self)
                
        self.background = Sprite("DATA/FormOptions2/2Bg.gi")
        self.background.setWidth(engine.screenWidth())
        self.background.setHeight(engine.screenHeight()) 
        self.background.setLayer(-2)
        self.addChild(self.background)

        deltaY = engine.screenHeight() - 768
        deltaX = engine.screenWidth() - 1024

        self.formBg = NinePatch("ORC/settings_bg.9.json")
        self.formBg.setGeometry(self.formBg.width() + deltaX, self.formBg.height() + deltaY)
        self.formBg.setPosition(math.floor((engine.screenWidth() - self.formBg.width()) / 2) - 10, 32)
        self.formBg.setLayer(-1)
        self.addChild(self.formBg)

        self.formTop = NinePatch("ORC/settings_top.9.json")
        self.formTop.setWidth(self.formTop.width() + deltaX)
        self.formTop.setPosition(math.floor((engine.screenWidth() - self.formTop.width()) / 2), 0)
        self.formTop.setLayer(-1)
        self.addChild(self.formTop)

        self.formBottom = NinePatch("ORC/settings_bottom.9.json")
        self.formBottom.setWidth(self.formBottom.width() + deltaX)
        self.formBottom.setPosition(math.floor((engine.screenWidth() - self.formTop.width()) / 2), engine.screenHeight() - self.formBottom.height())
        self.formBottom.setLayer(-1)
        self.addChild(self.formBottom)

        self.buttonNode = WidgetNode()
        self.buttonNode.setPosition(self.formBg.position().x + 80, self.formBg.position().y + 120)
        self.addWidget(self.buttonNode)

        self.spaceButton = Button("DATA/FormOptions2/2SpaceN.gi", "DATA/FormOptions2/2SpaceN.gi", "DATA/FormOptions2/2SpaceD.gi")
        self.spaceButton.setPosition(self.formBg.position().x + 26, self.formBg.position().y + 10)
        self.addWidget(self.spaceButton)
        
        self.okButton = Button("DATA/FormOptions2/2OkN.gi", "DATA/FormOptions2/2OkA.gi", "DATA/FormOptions2/2OkD.gi")
        self.okButton.setPosition(self.formBottom.position().x + 603 + deltaX, self.formBottom.position().y + 22)
        self.okButton.addListener(self)
        self.setSoundsDefault(self.okButton)
        self.addWidget(self.okButton)

        self.cancelButton = Button("DATA/FormOptions2/2CancelN.gi", "DATA/FormOptions2/2CancelA.gi", "DATA/FormOptions2/2CancelD.gi")
        self.cancelButton.setPosition(self.formBottom.position().x + 670 + deltaX, self.formBottom.position().y + 22)
        self.cancelButton.addListener(self)
        self.setSoundsDefault(self.cancelButton)       
        self.addWidget(self.cancelButton)

        self.autoButton = Button("DATA/FormOptions2/2AutoN.gi", "DATA/FormOptions2/2AutoA.gi", "DATA/FormOptions2/2AutoD.gi")
        self.autoButton.setPosition(self.formBottom.position().x + 102, self.formBottom.position().y + 22)
        self.setSoundsDefault(self.autoButton)
        self.addWidget(self.autoButton)
        self.highButton = Button("DATA/FormOptions2/2HighN.gi", "DATA/FormOptions2/2HighA.gi", "DATA/FormOptions2/2HighD.gi")
        self.highButton.setPosition(self.formBottom.position().x + 176, self.formBottom.position().y + 4)
        self.setSoundsDefault(self.highButton)  
        self.addWidget(self.highButton)
        self.mediumButton = Button("DATA/FormOptions2/2MediumN.gi", "DATA/FormOptions2/2MediumA.gi", "DATA/FormOptions2/2MediumD.gi")
        self.mediumButton.setPosition(self.formBottom.position().x + 170, self.formBottom.position().y + 33)
        self.setSoundsDefault(self.mediumButton)
        self.addWidget(self.mediumButton)
        self.lowButton = Button("DATA/FormOptions2/2LowN.gi", "DATA/FormOptions2/2LowA.gi", "DATA/FormOptions2/2LowD.gi")
        self.lowButton.setPosition(self.formBottom.position().x + 161, self.formBottom.position().y + 58)
        self.setSoundsDefault(self.lowButton)
        self.addWidget(self.lowButton)

        self.graphicsButton = Button("DATA/FormOptions2/2PageN.gi", "DATA/FormOptions2/2PageA.gi", "DATA/FormOptions2/2PageD.gi")
        self.graphicsButton.font = engine.coreFont()
        self.graphicsButton.text = _("Graphics")
        self.graphicsButton.color = 0xC4E0FF00
        self.graphicsButton.setPosition(0, 0)
        self.setSoundsDefault(self.graphicsButton)
        self.buttonNode.addWidget(self.graphicsButton)
        
        self.paramNode = WidgetNode()

        self.widthLabel = Label(_("Width:"))
        self.paramNode.addChild(self.widthLabel)
        self.widthEdit = LineEdit(engine.defaultSkin().lineEditStyle)
        self.paramNode.addWidget(self.widthEdit)
        self.heightLabel = Label(_("Height:"))
        self.paramNode.addChild(self.heightLabel)
        self.heightEdit = LineEdit(engine.defaultSkin().lineEditStyle)
        self.paramNode.addWidget(self.heightEdit)

        self.fullscreen = CheckBox(engine.defaultSkin().checkBoxStyle, _("Fullscreen mode"))
        self.paramNode.addWidget(self.fullscreen)
        self.useShaders = CheckBox(engine.defaultSkin().checkBoxStyle, _("Use shaders"))
        self.paramNode.addWidget(self.useShaders)

        self.widthEdit.setWidth(50)
        self.widthEdit.setPosition(100, 5)
        self.widthLabel.setPosition(5, 5 + (self.widthEdit.height() - self.widthLabel.font().size()))
        self.heightEdit.setWidth(50)
        self.heightEdit.setPosition(270, 5)
        self.heightLabel.setPosition(170, 5 + (self.widthEdit.height() - self.widthLabel.font().size()))
        self.fullscreen.setPosition(5, self.widthEdit.height() + 15)
        self.useShaders.setPosition(5, self.fullscreen.position().y + self.fullscreen.height() + 5)

        self.scroll = ScrollArea(engine.defaultSkin().scrollStyle, self.paramNode)
        self.scroll.setPosition(self.formBg.position().x + 257, self.formBg.position().y + 149)
        self.scroll.setGeometry(450 + deltaX, 440 + deltaY)
        self.addWidget(self.scroll)
        
        # RadioButton example
        #self.group = OpenSR.Engine.RadioButtonGroup()
        #self.r1 = OpenSR.Engine.RadioButton(engine.defaultSkin().radioButtonStyle, "Yes")
        #self.r2 = OpenSR.Engine.RadioButton(engine.defaultSkin().radioButtonStyle, "No")
        #self.r3 = OpenSR.Engine.RadioButton(engine.defaultSkin().radioButtonStyle, "I dunno lol")

        #self.r2.setPosition(0, self.r1.height() + 5)
        #self.r3.setPosition(0, self.r2.position().y + self.r2.height() + 5)
        
        #self.group.addRadioButton(self.r1)
        #self.group.addRadioButton(self.r2)
        #self.group.addRadioButton(self.r3)
        #self.group.setPosition(0, 300)
        
        #self.paramNode.addWidget(self.group)
        
        self.loadSettings()

    def loadSettings(self):
        self.widthEdit.setText(str(engine.propertiesGetInt("graphics.width", 1024)))
        self.heightEdit.setText(str(engine.propertiesGetInt("graphics.height", 768)))
        self.fullscreen.checked = engine.propertiesGetBool("graphics.fullscreen", False)
        self.useShaders.checked = engine.propertiesGetBool("graphics.useShaders", True)

    def saveSettings(self):
        width = int(self.widthEdit.text())
        height = int(self.heightEdit.text())
        fs = self.fullscreen.checked()
        shader = self.useShaders.checked()
        engine.propertiesSetInt("graphics.width", width)
        engine.propertiesSetInt("graphics.height", height)
        engine.propertiesSetBool("graphics.fullscreen", fs)
        engine.propertiesSetBool("graphics.useShaders", shader)
        
    def dispose(self):
        engine.removeWidget(self)
        
        self.cancelButton.removeListener(self)
        self.okButton.removeListener(self)
        
    def actionPerformed(self, action):
        if action.type() != ActionType.BUTTON_CLICKED:
            return

        if action.source() == self.okButton:               
            self.saveSettings()
            
        self.dispose()
        engine.addWidget(OpenSR.ORC.StartMenu.StartMenuWidget())
        
    def setSoundsDefault(self, button):
        clickSound = sound.loadSound("Sound/ButtonClick.wav")
        leaveSound = sound.loadSound("Sound/ButtonLeave.WAV")
        enterSound = sound.loadSound("Sound/ButtonEnter.wav")
        
        button.clickSound = clickSound
        button.enterSound = enterSound
        button.leaveSound = leaveSound   
