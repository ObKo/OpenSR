from OpenSR.Engine import ScriptWidget, Engine, ResourceManager, TiledPolyline, TiledBeizerCurve, \
                          BeizerCurve, Vector, Sprite
from OpenSR.World import WorldManager

engine = Engine.instance()
resources = ResourceManager.instance()

class PolyTestWidget(ScriptWidget):  
    P1_SPEED = 100
    P2_SPEED = 50
    def __init__(self):
        ScriptWidget.__init__(self)
        
        self.curve1 = BeizerCurve(Vector(100, 500), Vector(0, 100), Vector(600, 200), Vector(500, 500))
        self.line1 = TiledBeizerCurve('ORC/UnitPath2.png')
        self.line1.curve = self.curve1
        self.addChild(self.line1)
        
        self.curve2 = BeizerCurve(Vector(250, 250), Vector(500, 100), Vector(0, 100), Vector(500, 250))
        self.line2 = TiledBeizerCurve('ORC/UnitPath2.png')
        self.line2.curve = self.curve2
        self.addChild(self.line2)
        
        self.curve3 = BeizerCurve(Vector(100, 500), Vector(200, 400), Vector(300, 600), Vector(400, 500))
        self.line3 = TiledBeizerCurve('ORC/UnitPath2.png')
        self.line3.curve = self.curve3
        self.addChild(self.line3)
        
        self.curve3 = BeizerCurve(Vector(0, 500), Vector(400, 600), Vector(600, 600), Vector(1000, 500))
        self.line3 = TiledBeizerCurve('ORC/UnitPath2.png')
        self.line3.curve = self.curve3
        self.addChild(self.line3)
        
        self.p1up = True
        self.p2up = False
        
        self.p0s = Sprite('ORC/UnitPath12.png')
        self.p0s.position = self.curve3.p0
        self.addChild(self.p0s)
        
        self.p1s = Sprite('ORC/UnitPath1.png')
        self.p1s.position = self.curve3.p1
        self.addChild(self.p1s)
        
        self.p2s = Sprite('ORC/UnitPath1.png')
        self.p2s.position = self.curve3.p2
        self.addChild(self.p2s)
        
        self.p3s = Sprite('ORC/UnitPath1.png')
        self.p3s.position = self.curve3.p3
        self.addChild(self.p3s)
        
    
    def processLogic(self, dt):
        if self.p1up:
            self.curve3.p1.y = self.curve3.p1.y - self.P1_SPEED * dt / 1000.0
            if self.curve3.p1.y < 0:
                self.curve3.p1.y = 0
                self.p1up = False
        else:
            self.curve3.p1.y = self.curve3.p1.y + self.P1_SPEED * dt / 1000.0
            if self.curve3.p1.y > 768:
                self.curve3.p1.y = 768
                self.p1up = True
        
        if self.p2up:
            self.curve3.p2.y = self.curve3.p2.y - self.P2_SPEED * dt / 1000.0
            if self.curve3.p2.y < 0:
                self.curve3.p2.y = 0
                self.p2up = False
        else:
            self.curve3.p2.y = self.curve3.p2.y + self.P2_SPEED * dt / 1000.0
            if self.curve3.p2.y > 768:
                self.curve3.p2.y = 768
                self.p2up = True
       
        self.p1s.position = self.curve3.p1         
        self.p2s.position = self.curve3.p2
                
        self.line3.curve = self.curve3
    
    def dispose(self):
        pass
