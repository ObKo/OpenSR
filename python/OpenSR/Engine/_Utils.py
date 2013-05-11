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
    # TODO: Use path
    def find_module(self, fullname, path=None):
        splited =  fullname.split('.')
        if len(splited) < 2:
            return None
        if splited[0] != 'OpenSR':
            return None

        path_module = "%s.py" % fullname.replace('OpenSR.', '').replace('.', '/')
        path_package = "%s/__init__.py" % fullname.replace('OpenSR.', '').replace('.', '/')
        
        if ResourceManager.instance().resourceExists(path_module) or ResourceManager.instance().resourceExists(path_package):
            return self
        else:
            return None

    def load_module(self, fullname):
        if fullname in sys.modules:
            return sys.modules[fullname]
  
        is_package = False
        
        path = "%s.py" % fullname.replace('OpenSR.', '').replace('.', '/')
        if not ResourceManager.instance().resourceExists(path):
            is_package = True
            path = fullname.replace('OpenSR.', '').replace('.', '/')

        mod = imp.new_module(fullname)
                
        mod.__loader__ = self
        
        if not is_package:
            mod.__file__ = path
            sys.modules[fullname] = mod
            execPythonModule(mod.__file__, mod)
        else:
            mod.__file__ = "%s/__init__.py" % path
            mod.__path__ = [path]
            sys.modules[fullname] = mod
            execPythonModule(mod.__file__, mod)

        return mod
        
        
sys.stdout = OpenSROut()
sys.stderr = OpenSRErr() 
sys.meta_path.append(OpenSRImporter())
