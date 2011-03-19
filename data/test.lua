engine = Rangers.Engine:instance()
resources = Rangers.ResourceManager:instance()

backgroundTexture = resources:loadTexture(L"DATA/FormMain2/2main.gi")
background = nil
if Rangers.getPointer(backgroundTexture) then
  background = Rangers.Sprite:new(backgroundTexture)
end

shipAnimation = resources:loadAnimation(L"DATA/FormMain3/2Ship2.gai", true)
ship = nil
if Rangers.getPointer(shipAnimation) then
  ship = Rangers.AnimatedSprite:new(shipAnimation)
  ship:setPosition(0, engine:screenHeight() - ship:height())
end

stop = false;

engine:focusWidget(this)

function draw()
  if background then
    background:draw()
  end
  if ship then
    ship:draw()
  end
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
  r.x2 = background:width()
  r.y1 = 0.0
  r.y2 = background:height()
  return r
end

function mouseEnter()
  
end

function mouseLeave()
 
end

function mouseMove(x, y)
end

function processMain()
  if ship then
    ship:processMain()
  end
end

function processLogic(dt)
 if ship then
    ship:processLogic(dt)
  end
end