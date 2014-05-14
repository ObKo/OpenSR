from OpenSR.Engine import Engine, ResourceManager, ScriptWidget, ActionListener, Sprite, \
                          Action, Button, WidgetNode, ScrollArea, Label, SpriteXOrigin, \
                          SpriteYOrigin, Color, LabelWidget, ColorLabel, tr as _
from OpenSR.ORC.Utils import getCacheDATPath

engine = Engine.instance()
resources = ResourceManager.instance()

class QuestInfo:
    def __str__(self):
        return '{id: "' + self.id + '", ' + \
               'name: "' + self.name + '", ' + \
               'genre: "' + self.genre + '", ' + \
               'group: ' + str(self.group) + ', ' + \
               'access: ' + str(self.access) + ', ' + \
               'length: ' + str(self.length) + ', ' + \
               'image: "' + self.image + '", ' + \
               'difficulty: ' + str(self.difficulty) + '}'
    

class QuestSelectionWidget(ScriptWidget, ActionListener):
    def __init__(self):
        ScriptWidget.__init__(self)
        ActionListener.__init__(self)
        
        self.scrollStyle = resources.objectManager().getResourceObject('/world/skin/quest/dark').scrollStyle
        if not self.scrollStyle:
            self.scrollStyle = engine.defaultSkin.scrollStyle
            
        font = resources.objectManager().getResourceObject('/fonts/default')
        self.smallFont = resources.loadFont(font.path, 12, font.antialiasing)
        self.font = resources.loadFont(font)
        self.buttonFont = resources.loadFont(resources.objectManager().getResourceObject('/fonts/ranger-bold'))
        
        self.buttonSounds = (engine.defaultSkin.buttonStyle.clickSound,\
                             engine.defaultSkin.buttonStyle.enterSound,\
                             engine.defaultSkin.buttonStyle.leaveSound)
        
        self.bg = Sprite("DATA/FormLoadQuest/2BG.gi")
        self.bg.layer = -1
        self.addChild(self.bg)
        self.width = int(self.bg.width)
        self.height = int(self.bg.height)
        
        self.bg2 = Sprite("DATA/FormLoadQuest/2ImageBG.gi")
        self.bg2.layer = -3
        self.bg2.position = (640, 15)
        self.addChild(self.bg2)
        
        self.image = None
        
        self.descNode = WidgetNode()
        self.descScroll = ScrollArea(self.scrollStyle, self.descNode)
        self.descScroll.position = (631, 280)
        self.descScroll.geometry = (280, 243)
        self.addWidget(self.descScroll)
        
        self.desc = ColorLabel()
        self.desc.font = self.font
        self.desc.position = (0, 5)
        self.desc.wordWrap = True
        self.desc.setFixedWidth(280)
        self.desc.textColor = Color(0.0, 0.0, 0.0)
        self.desc.selectionColor = Color(0.0, 0.0, 1.0)
        self.descWidget = LabelWidget(self.desc)
        self.descNode.addWidget(self.descWidget)
        
        self.node = WidgetNode()
        
        self.scroll = ScrollArea(self.scrollStyle, self.node)
        self.scroll.position = (40, 145)
        self.scroll.geometry = (528, 460)
        self.addWidget(self.scroll)
        
        self.startButton = Button('DATA/FormSave2/2SaveN.gi', '', 'DATA/FormSave2/2SaveD.gi')
        self.startButton.sounds = (self.buttonSounds[0], '', '')
        self.startButton.text = _('Start', 'FormLoadQuest.Start', 'OpenSR-World')
        self.startButton.font = self.buttonFont
        self.startButton.color = Color.fromString('#002842')
        self.startButton.position = (638, 574)
        self.startButton.layer = 1
        self.startButton.addListener(self)
        self.addWidget(self.startButton)
        
        self.cancelButton = Button('DATA/FormSave2/2CancelN.gi', '', 'DATA/FormSave2/2CancelD.gi')
        self.cancelButton.sounds = (self.buttonSounds[0], '', '')
        self.cancelButton.text = _('Cancel', 'FormLoadQuest.Cancel', 'OpenSR-World')
        self.cancelButton.font = self.buttonFont
        self.cancelButton.color = Color.fromString('#002842')
        self.cancelButton.position = (638, 626)
        self.cancelButton.layer = 1
        self.cancelButton.addListener(self)
        self.addWidget(self.cancelButton)
        
        self.closeButton = Button('DATA/FormSave2/2CloseN.gi', 'DATA/FormSave2/2CloseA.gi', 'DATA/FormSave2/2CloseD.gi')
        self.closeButton.sounds = self.buttonSounds
        self.closeButton.position = (891, 601)
        self.closeButton.layer = 1
        self.closeButton.addListener(self)
        self.addWidget(self.closeButton)
        
        self.buttons = {}       
        self.loadQuests()
        
        self.selected = None
        
        first = None
        i = 0
        for q in self.quests:
            b = Button("DATA/FormLoadQuest/2SlotNormal.gi", "DATA/FormLoadQuest/2SlotOnMouse.gi", "DATA/FormLoadQuest/2SlotOnPress.gi")
            b.position = (0, 5 + i * (b.height + 5))
            self.node.addWidget(b)
            self.buttons[b] = i
            b.addListener(self)
            if i == 0:
                first = b
            
            nl = Label(q.name)
            nl.font = self.font
            nl.origin = (SpriteXOrigin.POSITION_X_LEFT, SpriteYOrigin.POSITION_Y_CENTER)
            nl.position = (15, int(5 + i * (b.height + 5) + b.height / 2))
            nl.layer = 1
            nl.color = Color.fromString("#A5B68C")
            self.node.addChild(nl)
            
            if q.length < 1:
                diff = "DATA/FormLoadQuest/2D1.gi"
            elif q.length < 2:
                diff = "DATA/FormLoadQuest/2D2.gi"
            elif q.length < 3:
                diff = "DATA/FormLoadQuest/2D3.gi"
            elif q.length < 4:
                diff = "DATA/FormLoadQuest/2D4.gi"
            else:
                diff = "DATA/FormLoadQuest/2D5.gi"
                
            diffIcon = Sprite(diff)
            diffIcon.origin = (SpriteXOrigin.POSITION_X_LEFT, SpriteYOrigin.POSITION_Y_CENTER)
            diffIcon.position = (int(b.width / 2.2), int(5 + i * (b.height + 5) + b.height / 2))
            diffIcon.layer = 1
            self.node.addChild(diffIcon)
            
            gl = Label(q.genre)
            gl.font = self.smallFont
            gl.origin = (SpriteXOrigin.POSITION_X_LEFT, SpriteYOrigin.POSITION_Y_CENTER)
            gl.position = (int(b.width / 2.2) + 5 + diffIcon.width, int(5 + i * (b.height + 5) + b.height / 2))
            gl.layer = 1
            gl.color = Color.fromString("#A5B68C")
            gl.wordWrap = True
            gl.setFixedWidth(b.width - int(b.width / 2.2) - 5 - diffIcon.width)
            self.node.addChild(gl)
            
            i += 1
            
        self.select(first)
    
    def select(self, button):
        if button == None:
            return

        if self.selected == button:
            return

        if self.selected != None:
            self.selected.removeListener(self)
            self.node.removeWidget(self.selected)
            i = self.buttons[self.selected]
            del self.buttons[self.selected]
            
            self.selected = Button("DATA/FormLoadQuest/2SlotNormal.gi", "DATA/FormLoadQuest/2SlotOnMouse.gi", "DATA/FormLoadQuest/2SlotOnPress.gi")
            self.selected.position = (0, 5 + i * (self.selected.height + 5))
            self.node.addWidget(self.selected)
            self.selected.addListener(self)
            self.buttons[self.selected] = i
        
        button.removeListener(self)
        self.node.removeWidget(button)
        i = self.buttons[button]
        del self.buttons[button]
        
        self.selected = Button("DATA/FormLoadQuest/2SlotActive.gi")
        self.selected.position = (0, 5 + i * (self.selected.height + 5))
        self.node.addWidget(self.selected)
        self.buttons[self.selected] = i
        self.selected.addListener(self)
        
        #if self.image != None:
            #self.removeChild(self.image)
            
        #self.image = Sprite(self.quests[i].image)
        #self.image.layer = -2
        #self.image.position = (640, 15)
        #self.image.geometry = (200, 228)
        #self.addChild(self.image)
        
        self.selectedQuest = self.quests[i].id
        
        text = resources.datRoot()['PlanetQuest']['StartText'][self.quests[i].id].value
        
        text = text.replace("<Player>", '\\cS' + self.rangerName + '\\cR')
        text = text.replace("<CurPlanet>", '\\cS' + self.fromPlanet + '\\cR')
        text = text.replace("<CurStar>", '\\cS' + self.fromStar + '\\cR')
        text = text.replace("<ToPlanet>", '\\cS' + self.toPlanet + '\\cR')
        text = text.replace("<ToStar>", '\\cS' + self.toStar + '\\cR')
        text = text.replace("<FromPlanet>", '\\cS' + self.fromPlanet + '\\cR')
        text = text.replace("<FromStar>", '\\cS' + self.fromStar + '\\cR')
        
        self.desc.text = text
        self.descScroll.markToUpdate()
        
        
    def loadQuests(self):
        dat = resources.datRoot()
        
        self.rangerName = dat['FormLoadQuest']['PRanger'].value
        self.toPlanet = dat['FormLoadQuest']['PToPlanet'].value
        self.toStar = dat['FormLoadQuest']['PToStar'].value
        self.fromPlanet = dat['FormLoadQuest']['PFromPlanet'].value
        self.fromStar = dat['FormLoadQuest']['PFromStar'].value
        
        self.quests = []                
        for q in dat['PlanetQuest']['List']:
            info = QuestInfo()
            info.id = q.name
            info.name = q['Name'].value
            info.genre = q['Genre'].value
            info.group = int(q['Group'].value)
            info.access = int(q['Access'].value)
            info.length = int(q['Length'].value)
            info.image = getCacheDATPath(q['Image'].value)
            info.difficulty = int(q['Dif'].value)
            self.quests.append(info)
            
    def dispose(self):
        for k in self.buttons.keys():
            k.removeListener(self)
        self.cancelButton.removeListener(self)
        self.startButton.removeListener(self)
        self.closeButton.removeListener(self)
            
    def processLogic(self, dt):
        ScriptWidget.processLogic(self, dt)
           
    def actionPerformed(self, action):
        if action.type != Action.Type.BUTTON_CLICKED:
            return
        
        if action.source in self.buttons:
            self.select(action.source)
        elif action.source == self.startButton:
            self.action(Action(self, Action.Type.DIALOG_ACCEPT))
        elif action.source == self.cancelButton:
            self.action(Action(self, Action.Type.DIALOG_CANCEL))
        elif action.source == self.closeButton:
            self.action(Action(self, Action.Type.DIALOG_CANCEL))