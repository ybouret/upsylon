from ctypes import *
m = cdll.LoadLibrary("./engine.dll")
EngineInit          = m.EngineInit
EngineInit.argtypes = [c_uint]
EngineInit.restype  = c_int

EngineQuit = m.EngineQuit
 
status = EngineInit(0)
print(status)



EngineQuit()

status = EngineInit(2)
print(status)


EngineQuit()
