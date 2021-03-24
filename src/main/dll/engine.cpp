
#include "y/rtld/soak.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/exception.hpp"
#include "y/ptr/arc.hpp"

#include <cmath>
#include <cstdio>

using namespace upsylon;


namespace  {
    
    typedef concurrent::simt SIMT;
    typedef arc_ptr<SIMT>    Threads;
    
    
    
    Y_SOAK_DERIVED(App,Threads);
    static int num_procs;
    inline App() : Threads( num_procs<= 0 ? new SIMT() : new SIMT(0,num_procs,1) )
    {
    }
    
    Y_SOAK_FINISH(App);
    
    int App::num_procs = 0;
    
    
}


static inline void enter() throw()
{
    soak::print(stderr,"Enter Engine DLL\n");
    soak::verbose = true;
}



static inline void leave() throw()
{
    soak::print(stderr, "Leave Engine DLL\n");
}


Y_DLL_SETUP(enter,leave);

Y_DLL_EXTERN()

Y_EXPORT int Y_DLL_API EngineInit(unsigned np) throw()
{
    soak::print(stderr,"EngineStart(%u)\n",np);
    App::num_procs = np;
    return App::Init() != 0 ? 0 : -1;
}

Y_EXPORT void Y_DLL_API EngineQuit() throw()
{
    App::Quit();
}



Y_DLL_FINISH()
