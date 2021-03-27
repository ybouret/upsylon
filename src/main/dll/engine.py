from ctypes import *

class Engine:

	def __init__(self,soname,num_procs):
		print('<Engine Python Init>')
		self.lib  = cdll.LoadLibrary(soname)
		self.Init = self.lib.EngineInit
		self.Quit = self.lib.EngineQuit
		self.Init.argtypes = [c_uint]
		self.Init.restype  = c_int
		if( 0 != self.Init(num_procs) ):
			print('error')
	
	def __del__(self):
		self.Quit()
		print('<Engine Python Quit')
		
		
