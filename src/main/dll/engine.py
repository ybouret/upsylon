from ctypes import *


class Engine:

    def __init__(self,soname):
        #load the library
        print('<Engine Python Load>')
        self.lib  = cdll.LoadLibrary(soname)
        
        # Quit function
        self.Quit = self.lib.EngineQuit

        # Init function, here take the NumProcs args
        self.__Init = self.lib.EngineInit
        self.__Init.argtypes = [c_uint]
        self.__Init.restype  =  c_int

        # Was Init function
        self.WasInit = self.lib.EngineWasInit
        self.WasInit.restype = c_int

        #if( 0 != self.Init(num_procs) ):
        #    raise OSError('C++ code failure')

        # average function, arguments are defined with numpy
        self.Average         = self.lib.EngineAverage
        self.Average.restype = c_double

        # process function, arguments are defined with numpy
        self.Process         = self.lib.EngineProcess
        self.Process.restype = c_int;
        
    def Init(self,NumProcs=0):
        if( 0 != self.__Init(NumProcs) ):
            raise OSError('C++ code failure')
            
    def __del__(self):
        self.Quit()
        print('<Engine Python Quit>')

