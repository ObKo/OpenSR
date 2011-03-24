engine = Rangers.Engine:instance()
resources = Rangers.ResourceManager:instance()

backgroundTexture = resources:loadTexture(L"DATA/FormMain3/2bg.gi")
background = Rangers.Sprite:new(backgroundTexture, this)
background:setWidth(background:width()/(engine:screenHeight()/background:height()))
background:setHeight(engine:screenHeight())

-- shipAnimation = resources:loadAnimation(L"DATA/FormMain3/2Ship1.gai", true, true)
shipAnimation = resources:loadTexture(L"DATA/FormMain3/2Ship1.gi")
-- ship = Rangers.AnimatedSprite:new(shipAnimation, this)
ship = Rangers.Sprite:new(shipAnimation, this)
ship:setPosition(0, engine:screenHeight() - ship:height())

stop = false
t = 0
bgspeed = 5/1000
bgforward = true

engine:focusWidget(this)

function draw()
  background:draw()
  ship:draw()
end

function keyPressed(k)
  if stop then
    ship:start()
    stop = false
  else
    ship:stop()
    stop = true
  end
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
  ship:delete()
  ship = nil
end

function processMain()
  ship:processMain()
  background:processMain()
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
  t = t + dt
end