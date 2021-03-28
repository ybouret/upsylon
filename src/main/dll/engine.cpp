
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
typedef memory::shack    Cache;
typedef memory::shacks   Caches;

 

//------------------------------------------------------------------------------
//
// Declare internal Code
//
//------------------------------------------------------------------------------

Y_SOAK_DERIVED2(Engine,Threads,concurrent::runnable);


// some declaration(s) to communitcate
static int num_procs;

Caches caches;

// the constructor
inline Engine() :
Threads( num_procs<= 0 ? new SIMT() : new SIMT(0,num_procs,1) ),
caches( (**this).size(), upsylon::memory::shacks::construct_filled )
{
    soak::print(stderr,"#threads=%u\n", unsigned( (**this).size() ) );
    caches.make<double>();
}

// the C++ functions
inline double Average(const double  *source,
                      const unsigned length) throw()
{
    Y_SOAK_VERBOSE(soak::print(stderr, "<Engine::Average[%u]>\n",length));
    struct ops
    {
    };
    
    return 0;
    
}

inline virtual void run(const concurrent::context &ctx, lockable &) throw()
{
}


Y_SOAK_FINISH(Engine,unsigned np,Engine::num_procs=np);

int Engine::num_procs = 0;

//------------------------------------------------------------------------------
//
// done
//
//------------------------------------------------------------------------------

// C-interfacing
Y_DLL_EXTERN()

Y_EXPORT double Y_DLL_API EngineAverage(const double *source, const unsigned length) throw()
{
    return Engine::_().Average(source,length);
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

