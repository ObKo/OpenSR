background = Rangers.Sprite:new_local(L"DATA/FormMain3/2bg.gi", this)
background:setWidth(background:width()/(engine:screenHeight()/background:height()))
background:setHeight(engine:screenHeight())
background:setLayer(-3)

bgPlanet = Rangers.Sprite:new_local(L"DATA/FormMain3/2Planet.gi", this)
bgPlanet:setOrigin(Rangers.POSITION_X_LEFT, Rangers.POSITION_Y_BOTTOM)
bgPlanet:setPosition(0, engine:screenHeight())
bgPlanet:setLayer(-2)

bgAnimLine = Rangers.GAISprite:new_local(L"DATA/FormMain2/2AnimLine.gai", this)
bgAnimLine:setOrigin(Rangers.POSITION_X_RIGHT, Rangers.POSITION_Y_TOP)
bgAnimLine:setPosition(engine:screenWidth(), 0)
bgAnimLine:setLayer(0)

bgAnim = Rangers.GAISprite:new_local(L"DATA/FormMain2/2AnimMain.gai", this)
bgAnim:setPosition(0, 0)
bgAnim:setLayer(-1)
bgAnim:setWidth(engine:screenWidth())
bgAnim:setHeight(engine:screenHeight())

caption = Rangers.Sprite:new_local(L"DATA/FormMain3/2caption.gi", this)
caption:setOrigin(Rangers.POSITION_X_RIGHT, Rangers.POSITION_Y_CENTER)
caption:setPosition(engine:screenWidth(), 110)
caption:setLayer(1)

ship = Rangers.GAISprite:new_local(L"DATA/FormMain3/2Ship1.gai", this)
ship:setPosition(0, engine:screenHeight() - ship:height())
ship:setLayer(1)

menuNode = Rangers.WidgetNode:new_local(this)
menuNode:setPosition(engine:screenWidth() - 400, 300);
menuNode:setLayer(2)

buttons = {}
buttons["newGame"] = Rangers.Button:new_local(L"NewGameButtonNormal.gi", L"NewGameButtonHovered.gi", L"NewGameButtonPressed.gi", menuNode);
buttons["newGame"]:setPosition(0, 0)
h = buttons["newGame"]:height()

buttons["load"] = Rangers.Button:new_local(L"LoadButtonNormal.gi", L"LoadButtonHovered.gi", L"LoadButtonPressed.gi", menuNode);
buttons["load"]:setPosition(0, h + 15)
buttons["settings"] = Rangers.Button:new_local(L"SettingsButtonNormal.gi", L"SettingsButtonHovered.gi", L"SettingsButtonPressed.gi", menuNode);
buttons["settings"]:setPosition(0, 2 * (h + 15))
buttons["records"] = Rangers.Button:new_local(L"RecordsButtonNormal.gi", L"RecordsButtonHovered.gi", L"RecordsButtonPressed.gi", menuNode);
buttons["records"]:setPosition(0, 3 * (h + 15))
buttons["about"] = Rangers.Button:new_local(L"AboutButtonNormal.gi", L"AboutButtonHovered.gi", L"AboutButtonPressed.gi", menuNode);
buttons["about"]:setPosition(0, 4 * (h + 15))
buttons["exit"] = Rangers.Button:new_local(L"ExitButtonNormal.gi", L"ExitButtonHovered.gi", L"ExitButtonPressed.gi", menuNode);
buttons["exit"]:setPosition(0, 5 * (h + 15))

for k,v in pairs(buttons) do
  v:addListener(actionListener)
end

t = 0
bgspeed = 20/1000
bgforward = true

function actionPerformed(action)
    if action:source() == buttons["exit"] then
        engine:quit(0)
    end
    if action:source() == buttons["settings"] then
        engine:markWidgetDeleting(this);
    end
end

function processLogic(dt)
  if math.abs(t * bgspeed) >= background:width() - engine:screenWidth() then
    t = 0
    bgforward = not bgforward
  end
  if bgforward then
    background:setPosition(- t * bgspeed, 0)
  else 
    background:setPosition(-background:width() + engine:screenWidth() + t * bgspeed, 0)
  end
  t = t + dt
end