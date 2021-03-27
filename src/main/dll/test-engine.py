from ctypes import *
from engine import Engine

engine = Engine("./engine.dll",0)
 
engine.Quit()

engine.Init(2)
