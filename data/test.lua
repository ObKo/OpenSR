engine = Rangers.getEngine()
resources = Rangers.getResourceManager()
texture = resources:loadTexture(L"DATA/Star/Star00.gi")
star = Rangers.Sprite:new(texture)

function draw()
  star:draw()
end

function keyPressed(k)
  l = key.unicode
end

function getBoundingRect()
  r = Rangers.Rect:new()
  r.x1 = 0
  r.x2 = 1024
  r.y1 = 0
  r.y2 = 768
  return r
end

function mouseEnter()
  
end

function mouseLeave()
  
end

function mouseMove(x, y)
  luaDebug(L("X: " + x))
end