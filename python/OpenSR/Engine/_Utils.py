from ._Engine import *
import sys

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
        
sys.stdout = OpenSROut()
sys.stderr = OpenSRErr() 
