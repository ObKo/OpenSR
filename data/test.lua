engine = Rangers.getEngine()
resources = Rangers.getResourceManager()
texture = resources:loadAnimation(L"DATA/Star/Star00.gai")
star = Rangers.AnimatedSprite:new(texture)

function draw()
  star:draw()
end

function keyPressed(k)
  l = key.unicode
end

function getBoundingRect()
  r = Rangers.Rect:new()
  r.x1 = 0.0
  r.x2 = star:width()
  r.y1 = 0.0
  r.y2 = star:height()
  return r
end

function mouseEnter()
  star:start()
end

function mouseLeave()
  star:stop()
end

function mouseMove(x, y)
end

function processMain()
  star:processMain()
end

function processLogic(dt)
  star:processLogic(dt)
end