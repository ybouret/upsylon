from ctypes import * 
import numpy as np
import platform


module_name  = "./libmodule.so"
if platform.system() == "Windows":
	module_name="./libmodule.dll"

m            = cdll.LoadLibrary(module_name)
Sin          = m.Sin
Sin.argtypes = [c_double]
Sin.restype  = c_double

for i in np.arange(0,3,0.5):
	print(i,Sin(i))

