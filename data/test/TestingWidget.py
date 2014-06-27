from OpenSR.Engine import ScriptWidget, ActionListener, Engine, ResourceManager, \
                          SoundManager, RadioButton, RadioButtonGroup, Action
from OpenSR.World import WorldManager

from OpenSR.test.Poly import PolyTestWidget

engine = Engine.instance()
resources = ResourceManager.instance()
sound = SoundManager.instance()
world = WorldManager.instance()

class TestingWidget(ScriptWidget, ActionListener):  
    TESTS = ['None', 'Polyline']
    
    def __init__(self):
        ScriptWidget.__init__(self)
        ActionListener.__init__(self)
        
        self.widget = None
        self.testBtnGroup = RadioButtonGroup()
        self.testBtns = []
        self.layer = 100
        self.addWidget(self.testBtnGroup)
        i = 0
        
        for test in self.TESTS:
            b = RadioButton(engine.defaultSkin.radioButtonStyle, test)
            b.position = (10, 50 + i * (b.height + 5))
            self.testBtnGroup.addRadioButton(b)
            self.testBtns.append(b)
            b.addListener(self)
            i = i + 1
        
    def actionPerformed(self, action):
        if action.type != Action.Type.RADIOBUTTON_SWITCHED:
            return

        testId = self.TESTS[self.testBtns.index(action.source)]
        
        if testId == 'None' and self.widget != None:
            self.removeWidget(self.widget)
            self.widget.dispose()
            self.widget = None
        elif testId == 'Polyline':
            self.widget = PolyTestWidget()
            self.addWidget(self.widget)
