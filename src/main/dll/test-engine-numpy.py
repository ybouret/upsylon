from ctypes import *from engine import Engineimport numpy as npengine = Engine("./engine.dll")if not engine.WasInit():    engine.Init(4)engine.Average.argtypes = [np.ctypeslib.ndpointer(dtype=np.float64,ndim=1,flags='C_CONTIGUOUS')]arr = np.array([1,2,3.2],dtype=np.float64)print('array',arr)print('average',engine.Average(arr,arr.size))engine.Process.argtypes = [    np.ctypeslib.ndpointer(dtype=np.float64,ndim=1,flags='C_CONTIGUOUS'),    np.ctypeslib.ndpointer(dtype=np.float64,ndim=1,flags='C_CONTIGUOUS')    ]brr = np.zeros(3,dtype=np.float64)print('source',brr)result=engine.Process(brr,arr,arr.size);print('result',result)print('return',brr)