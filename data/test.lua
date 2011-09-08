engine = Rangers.Engine:instance()
resources = Rangers.ResourceManager:instance()

backgroundTexture = resources:loadTexture(L"DATA/FormMain3/2bg.gi")
background = Rangers.Sprite:new(backgroundTexture, this)
background:setWidth(background:width()/(engine:screenHeight()/background:height()))
background:setHeight(engine:screenHeight())

bgPlanetTexture = resources:loadTexture(L"DATA/FormMain3/2Planet.gi")
bgPlanet = Rangers.Sprite:new(bgPlanetTexture, this, Rangers.TEXTURE_NORMAL,
    Rangers.POSITION_X_LEFT, Rangers.POSITION_Y_BOTTOM)
bgPlanet:setPosition(0, engine:screenHeight())

bgAnimLine = resources:getAnimatedSprite(L"DATA/FormMain2/2AnimLine.gai", true, true, this)

shipAnimation = resources:loadAnimation(L"DATA/FormMain3/2Ship1.gai", true, true)
-- shipAnimation = resources:loadTexture(L"DATA/FormMain3/2Ship1.gi")
ship = Rangers.AnimatedSprite:new(shipAnimation, this)
-- ship = Rangers.Sprite:new(shipAnimation, this)
ship:setPosition(0, engine:screenHeight() - ship:height())

t = 0
bgspeed = 5/1000
bgforward = true

engine:focusWidget(this)

function draw()
  background:draw()
  bgPlanet:draw()
--   bgAnimLine:draw()
  ship:draw()
end

function keyPressed(k)
  bgAnimLine:delete()
  bgAnimLine = nil
end

function getBoundingRect()
  r = Rangers.Rect:new()
  r.x1 = 0.0
  r.x2 = ship:width()
  r.y1 = 0.0
  r.y2 = ship:height()
  return r
end

function mouseEnter()
end

function mouseLeave()
end

function mouseMove(x, y)
end

function mouseUp(key, x, y)
end

function mouseDown(key, x, y)
end

function mouseClick(x, y)

end

function processMain()
  ship:processMain()
  bgPlanet:processMain()
  background:processMain()
--   bgAnimLine:processMain()
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
  ship:processLogic(dt)
--   bgAnimLine:processLogic(dt)
  t = t + dt
end