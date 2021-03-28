from ctypes import *


class Engine:

    def __init__(self,soname,num_procs):
        print('<Engine Python Init>')
        self.lib  = cdll.LoadLibrary(soname)
        # Quit function
        self.Quit = self.lib.EngineQuit

        # Init function
        self.Init = self.lib.EngineInit
        self.Init.argtypes = [c_uint]
        self.Init.restype  =  c_int
        if( 0 != self.Init(num_procs) ):
            raise OSError('C++ code failure')

        # average function
        self.Average = self.lib.EngineAverage
        self.Average.restype = c_double

    def __del__(self):
        self.Quit()
        print('<Engine Python Quit>')

