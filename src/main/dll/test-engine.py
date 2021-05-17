from ctypes import *
from engine import Engine

engine = Engine("./engine.dll")
print('WasInit=',engine.WasInit())

if not engine.WasInit():
    engine.Init(2);
    
print('WasInit=',engine.WasInit())
print('hash',engine.Hash(b'Hello'))
