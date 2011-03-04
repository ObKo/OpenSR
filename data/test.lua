engine = Rangers.Engine:instance()
resources = Rangers.ResourceManager:instance()

backgroundTexture = resources:loadTexture(L"DATA/FormMain2/2main.gi")
background = nil
if Rangers.getPointer(backgroundTexture) then
  background = Rangers.Sprite:new(backgroundTexture)
end

function draw()
  if background then
    background:draw()
  end
end

function keyPressed(k)
  
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
  
end

function processLogic(dt)
  
end