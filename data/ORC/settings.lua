background = Sprite(L"DATA/FormOptions2/2Bg.gi", this)
background:setWidth(engine:screenWidth())
background:setHeight(engine:screenHeight()) 
background:setLayer(-2)

deltaY = engine:screenHeight() - 768
deltaX = engine:screenWidth() - 1024

formBg = NinePatch(L"ORC/settings_bg.9.json", this)
formBg:setGeometry(formBg:width() + deltaX, formBg:height() + deltaY)
formBg:setPosition(math.floor((engine:screenWidth() - formBg:width()) / 2) - 10, 32)
formBg:setLayer(-1)

formTop = NinePatch(L"ORC/settings_top.9.json", this)
formTop:setWidth(formTop:width() + deltaX)
formTop:setPosition(math.floor((engine:screenWidth() - formTop:width()) / 2), 0)
formTop:setLayer(-1)

formBottom = NinePatch(L"ORC/settings_bottom.9.json", this)
formBottom:setWidth(formBottom:width() + deltaX)
formBottom:setPosition(math.floor((engine:screenWidth() - formTop:width()) / 2), engine:screenHeight() - formBottom:height())
formBottom:setLayer(-1)

buttonNode = WidgetNode(this)
buttonNode:setPosition(formBg:position().x + 80, formBg:position().y + 120)

spaceButton = Button(L"DATA/FormOptions2/2SpaceN.gi", L"DATA/FormOptions2/2SpaceN.gi", L"DATA/FormOptions2/2SpaceD.gi", this)
spaceButton:setPosition(formBg:position().x + 26, formBg:position().y + 10)

okButton = Button(L"DATA/FormOptions2/2OkN.gi", L"DATA/FormOptions2/2OkA.gi", L"DATA/FormOptions2/2OkD.gi", this)
okButton:setPosition(formBottom:position().x + 608 + deltaX, formBottom:position().y + 22)
okButton:addListener(actionListener)
okButton:setSounds(L"Sound/ButtonClick.wav", L"Sound/ButtonLeave.WAV", L"Sound/ButtonEnter.wav")

cancelButton = Button(L"DATA/FormOptions2/2CancelN.gi", L"DATA/FormOptions2/2CancelA.gi", L"DATA/FormOptions2/2CancelD.gi", this)
cancelButton:setPosition(formBottom:position().x + 675 + deltaX, formBottom:position().y + 22)
cancelButton:addListener(actionListener)
cancelButton:setSounds(L"Sound/ButtonClick.wav", L"Sound/ButtonLeave.WAV", L"Sound/ButtonEnter.wav")

autoButton = Button(L"DATA/FormOptions2/2AutoN.gi", L"DATA/FormOptions2/2AutoA.gi", L"DATA/FormOptions2/2AutoD.gi", this)
autoButton:setPosition(formBottom:position().x + 102, formBottom:position().y + 22)
autoButton:setSounds(L"Sound/ButtonClick.wav", L"Sound/ButtonLeave.WAV", L"Sound/ButtonEnter.wav")
highButton = Button(L"DATA/FormOptions2/2HighN.gi", L"DATA/FormOptions2/2HighA.gi", L"DATA/FormOptions2/2HighD.gi", this)
highButton:setPosition(formBottom:position().x + 176, formBottom:position().y + 4)
highButton:setSounds(L"Sound/ButtonClick.wav", L"Sound/ButtonLeave.WAV", L"Sound/ButtonEnter.wav")
mediumButton = Button(L"DATA/FormOptions2/2MediumN.gi", L"DATA/FormOptions2/2MediumA.gi", L"DATA/FormOptions2/2MediumD.gi", this)
mediumButton:setPosition(formBottom:position().x + 170, formBottom:position().y + 33)
mediumButton:setSounds(L"Sound/ButtonClick.wav", L"Sound/ButtonLeave.WAV", L"Sound/ButtonEnter.wav")
lowButton = Button(L"DATA/FormOptions2/2LowN.gi", L"DATA/FormOptions2/2LowA.gi", L"DATA/FormOptions2/2LowD.gi", this)
lowButton:setPosition(formBottom:position().x + 161, formBottom:position().y + 58)
lowButton:setSounds(L"Sound/ButtonClick.wav", L"Sound/ButtonLeave.WAV", L"Sound/ButtonEnter.wav")

graphicsButton = Button(L"DATA/FormOptions2/2PageN.gi", L"DATA/FormOptions2/2PageA.gi", L"DATA/FormOptions2/2PageD.gi", buttonNode)
graphicsButton:setFont(engine:coreFont());
graphicsButton:setText(_("Graphics"))
graphicsButton:setColor(0x00C4E0FF)
graphicsButton:setPosition(0, 0)
graphicsButton:setSounds(L"Sound/ButtonClick.wav", L"Sound/ButtonLeave.WAV", L"Sound/ButtonEnter.wav")
paramNode = WidgetNode()

widthLabel = Label(_("Width:"), paramNode)
widthEdit = LineEditWidget(engine:defaultSkin().lineEditStyle, paramNode)
heightLabel = Label(_("Height:"), paramNode)
heightEdit = LineEditWidget(engine:defaultSkin().lineEditStyle, paramNode)

fullscreen = CheckBox(engine:defaultSkin().checkBoxStyle, _("Fullscreen mode"), paramNode)
test = Button(L"DATA/PlanetBG/2/100.gi", paramNode)

widthEdit:setWidth(50)
widthEdit:setPosition(100, 5)
widthLabel:setPosition(5, 5 + (widthEdit:height() - widthLabel:font():size()) / 2)
heightEdit:setWidth(50)
heightEdit:setPosition(270, 5)
heightLabel:setPosition(170, 5 + (widthEdit:height() - widthLabel:font():size()) / 2)
fullscreen:setPosition(5, widthEdit:height() + 15)

scroll = ScrollArea(engine:defaultSkin().scrollStyle, paramNode, this)
scroll:setPosition(formBg:position().x + 257, formBg:position().y + 149)
scroll:setGeometry(450 + deltaX, 440 + deltaY)

function loadSettings()
  widthEdit:setText(L(tostring(propertiesGetInt("graphics.width", 1024))))
  heightEdit:setText(L(tostring(propertiesGetInt("graphics.height", 768))))
  fullscreen:setChecked(propertiesGetBool("graphics.fullscreen", false))
end

function saveSettings()
  width = tonumber(toString(widthEdit:text()))
  height = tonumber(toString(heightEdit:text()))
  fs = fullscreen:checked()
  propertiesSetInt("graphics.width", width)
  propertiesSetInt("graphics.height", height)
  propertiesSetBool("graphics.fullscreen", fs)
end

function dispose()
    this:dispose();
    widget = LuaWidget(L"ORC/startmenu.lua")
    engine:addWidget(widget)
end

function actionPerformed(action)
    if action:type() ~= Action.BUTTON_CLICKED then
        return
    end
    if action:source() == okButton then
        saveSettings()
        dispose()
    end
    if action:source() == cancelButton then
        dispose()
    end
end

loadSettings()