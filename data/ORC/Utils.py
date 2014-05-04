from OpenSR.World import PlanetStyle, WorldManager, StyleManager
from OpenSR.Engine import ResourceManager, Color

world = WorldManager.instance()
resources = ResourceManager.instance()

def loadPlanetStylesFromDAT():    
    datRoot = resources.datRoot()
    for p in datRoot['Data']['SE']['Planet']:
        style = PlanetStyle()
    
        style.id = p.name
        image = resources.datValue(p['Image'].value)
        style.texture = image.replace('\\', '/').replace('psd', 'png').replace('data', 'ORC')
        try:
            cloud = p['Cloud0'].value.split(',')
            image = resources.datValue(cloud[1])
            style.cloudSpeed = - float(cloud[0]) * 20
            style.cloud = image.replace('\\', '/').replace('.psd', '.png').replace('data', 'ORC')
            style.hasCloud = True
        except:
            style.hasCloud = False
        style.speed = -12
        style.hasRing = False
        style.hasRingBackground = False
        
        try:
            color = p['AtmColor'].value.split(',')
        except:
            color = (255, 255, 255)
                    
        style.ambientColor = Color(int(color[0]) / 255.0, int(color[1]) / 255.0, int(color[2]) / 255.0)
        world.styleManager().addPlanetStyle(style)
