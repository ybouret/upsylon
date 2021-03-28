
#include "y/rtld/soak.hpp"
#include "y/concurrent/loop/runic.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/exception.hpp"
#include "y/ptr/arc.hpp"
#include "y/memory/shacks.hpp"

#include <cmath>
#include <cstdio>

using namespace upsylon;



typedef concurrent::simt  SIMT;
typedef concurrent::runic Threads;
 

//------------------------------------------------------------------------------
//
// Declare internal Code
//
//------------------------------------------------------------------------------

Y_SOAK_DERIVED(Engine,Threads);


// some declaration(s) to communitcate
static int num_procs;


// the constructor
inline Engine() :
Threads( num_procs<= 0 ? new SIMT() : new SIMT(0,num_procs,1) )
{
    soak::print(stderr,"#threads=%u\n", unsigned( (**this).size() ) );
    make<double>();
}

// the C++ functions
inline double Average(const double  *source,
                      const unsigned length) throw()
{
    Y_SOAK_VERBOSE(soak::print(stderr, "<Engine::Average[%u]>\n",length));
 
    struct ops
    {
        const double  *source;
        unsigned       length;
        static inline void run(const concurrent::context &cntx,
                               memory::shack             &data,
                               void                      *args,
                               lockable &sync) throw()
        {
           
            double local_sum = 0;
            
            {
                ops     &self   = *static_cast<ops *>(args);
                unsigned offset = 0;
                unsigned length = self.length;
                cntx.split(length,offset);
                
                if(soak::verbose)
                {
                    Y_LOCK(sync);
                    soak::print(stderr, "\t<Engine::Average@%s: #%u>\n", cntx.label, length);
                }
                
                const double *source = self.source;
                while(length-- > 0 )
                {
                    local_sum += source[offset++];
                }
            }
            
            data.as<double>() = local_sum;
        }
        
    };
    
    ops task = { source, length };
    
    (*this)( ops::run, &task );
    
    return sum<double>()/length;
    
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

