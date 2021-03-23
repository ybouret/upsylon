from ctypes import * 
import numpy as np

m            = cdll.LoadLibrary("./libmodule.so")
Sin          = m.Sin
Sin.argtypes = [c_double]
Sin.restype  = c_double

for i in np.arange(0,3,0.1):
	print(i,Sin(i))

