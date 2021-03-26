
#include "y/rtld/soak.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/exception.hpp"
#include "y/ptr/arc.hpp"

#include <cmath>
#include <cstdio>

using namespace upsylon;



typedef concurrent::simt SIMT;
typedef arc_ptr<SIMT>    Threads;



Y_SOAK_DERIVED(Engine,Threads);

// some declaration(s)
static int num_procs;

// the constructor
inline Engine() : Threads( num_procs<= 0 ? new SIMT() : new SIMT(0,num_procs,1) )
{
    soak::print(stderr,"#threads=%u\n", unsigned( (**this).size() ) );
}

Y_SOAK_FINISH(Engine,unsigned np,Engine::num_procs=np);

int Engine::num_procs = 0;




static inline void enter() throw()
{
    soak::print(stderr,"<%s.dll>\n",Engine::call_sign);
    soak::verbose = true;
}



static inline void leave() throw()
{
    soak::print(stderr,"<%s.dll/>\n",Engine::call_sign);
}


Y_DLL_SETUP(enter,leave);

