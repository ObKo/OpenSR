from ._Engine import *
import sys
import imp

class OpenSROut:
    def __init__(self):
        self.data = ''
    def write(self, stuff):
        self.data += stuff
        lines = self.data.split('\n')
        
        if len(lines) <= 1:
            return
        
        self.data = lines[-1]
        lines.pop()
        
        for line in lines:
            pythonOut(line)
            
    def flush(self):
        pythonOut(self.data)
        self.data = ''

class OpenSRErr:
    def __init__(self):
        self.data = ''
    def write(self, stuff):
        self.data += stuff
        lines = self.data.split('\n')
        
        if len(lines) <= 1:
            return
        
        self.data = lines[-1]
        lines.pop()
        
        for line in lines:
            pythonError(line)
            
    def flush(self):
        pythonError(self.data)
        self.data = ''
        
class OpenSRImporter:
    def find_module(self, fullname, path=None):
        splited =  fullname.split('.')
        if len(splited) < 2:
            return None
        if splited[0] != 'OpenSR':
            return None

        path = "%s.py" % fullname.replace('OpenSR.', '').replace('.', '/')
        
        if ResourceManager.instance().resourceExists(path):
            return self
        else:
            return None

    def load_module(self, fullname):
        if fullname in sys.modules:
            return sys.modules[fullname]
        mod = imp.new_module(fullname)
        
        path = "%s.py" % fullname.replace('OpenSR.', '').replace('.', '/')
        
        mod.__loader__ = self
        mod.__file__ = "%s.py" % fullname.rsplit('.')[0]

        execPythonModule(path, mod)
        
        sys.modules[fullname] = mod
        return mod
        
        
sys.stdout = OpenSROut()
sys.stderr = OpenSRErr() 
sys.meta_path.append(OpenSRImporter())
