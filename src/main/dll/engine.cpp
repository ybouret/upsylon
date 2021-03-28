
#include "y/rtld/soak.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/exception.hpp"
#include "y/ptr/arc.hpp"
#include "y/memory/shacks.hpp"

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

// interfacing
Y_DLL_EXTERN()

Y_EXPORT double Y_DLL_API EngineAverage(const double *arr, const unsigned n) throw()
{
    Engine *engine = Engine::_(); (void)engine;
    assert(arr); assert(n>0);
    double sum = 0;
    for(unsigned i=0;i<n;++i) { sum += arr[i]; }
    return sum/n;
}

Y_DLL_FINISH()


static inline void enter() throw()
{
    soak::print(stderr,"<C++ %s.dll Enter>\n",Engine::call_sign);
    soak::verbose = true;
}



static inline void leave() throw()
{
    soak::print(stderr,"<C++ %s.dll Leave/>\n",Engine::call_sign);
}


Y_DLL_SETUP(enter,leave);

