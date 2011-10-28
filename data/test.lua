background = Rangers.Sprite:new(L"DATA/FormMain3/2bg.gi", this)
background:setWidth(background:width()/(engine:screenHeight()/background:height()))
background:setHeight(engine:screenHeight())
background:setLayer(-1)
tolua.takeownership(background)

bgPlanet = Rangers.Sprite:new(L"DATA/FormMain3/2Planet.gi", this)
bgPlanet:setOrigin(Rangers.POSITION_X_LEFT, Rangers.POSITION_Y_BOTTOM)
bgPlanet:setPosition(0, engine:screenHeight())
bgPlanet:setLayer(0)
tolua.takeownership(bgPlanet)

bgAnimLine = Rangers.GAISprite:new(L"DATA/FormMain2/2AnimLine.gai", this)
bgAnimLine:setOrigin(Rangers.POSITION_X_RIGHT, Rangers.POSITION_Y_TOP)
bgAnimLine:setPosition(engine:screenWidth(), 0)
bgAnimLine:setLayer(0)
tolua.takeownership(bgAnimLine)

caption = Rangers.Sprite:new(L"DATA/FormMain2/2AnimCaption.gi", this)
caption:setOrigin(Rangers.POSITION_X_RIGHT, Rangers.POSITION_Y_TOP)
caption:setPosition(engine:screenWidth(), 0)
caption:setLayer(1)
tolua.takeownership(caption)

ship = Rangers.GAISprite:new(L"DATA/FormMain3/2Ship1.gai", this)
ship:setPosition(0, engine:screenHeight() - ship:height())
ship:setLayer(1)
tolua.takeownership(ship)

exitButton = Rangers.Button:new(L"DATA/FormMain2/2ButExitN.gi", L"DATA/FormMain2/2ButExitA.gi", L"DATA/FormMain2/2ButExitD.gi", this);
exitButton:setPosition(100, 100)
exitButton:addListener(actionListener)
tolua.takeownership(exitButton)

t = 0
bgspeed = 5/1000
bgforward = true

function getBoundingRect()
  r = Rangers.Rect:new()
  r.x1 = 0.0
  r.x2 = ship:width()
  r.y1 = 0.0
  r.y2 = ship:height()
  return r
end

function actionPerformed(action)
    if action:source() == exitButton then
        engine:quit(0)
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