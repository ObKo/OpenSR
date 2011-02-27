engine = Rangers.getEngine()
resources = Rangers.getResourceManager()
startexture = resources:loadAnimation(L("DATA/Star/Terron_Transform.gai"))
robottexture = resources:loadAnimation(L("DATA/Items/2RepairRobot0a.gai"))
if Rangers.getPointer(startexture) then
    star = Rangers.AnimatedSprite:new(startexture)
    engine:addObject(star)
end
if Rangers.getPointer(robottexture) then
    robot = Rangers.AnimatedSprite:new(robottexture)
    engine:addObject(robot)
end