#include "y/concurrent/at-exit.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/os/error.hpp"
#include "y/type/block/swap.hpp"
#include <cerrno>
#include <cstring>
#include <cstdlib>

namespace upsylon
{
    namespace
    {
        
        struct __at_exit
        {
            at_exit::procedure  proc;
            at_exit::longevity  when;
            
            static __at_exit tasks[at_exit::stack_size];
            static size_t    count;
            static bool      registered;
            
            static inline void call() throw()
            {
                for(size_t i=0;i<count;++i)
                {
                    __at_exit &cb = tasks[i];
                    assert(cb.proc);
                    try{cb.proc();} catch(...) {}
                    memset(&cb,0,sizeof(__at_exit));
                }
            }
            
            static inline void check() throw()
            {
                if(count>=2)
                {
                    __at_exit *curr = &tasks[count-1];
                    __at_exit *prev = &tasks[count-2];
                    while(curr>tasks && prev->when>=curr->when)
                    {
                        bswap(*prev,*curr);
                        --prev;
                        --curr;
                    }
#if !defined(NDEBUG)
                    for(size_t i=1;i<count;++i)
                    {
                        assert(tasks[i-1].when<=tasks[i].when);
                    }
#endif
                }
            }
            
        };
        
        size_t    __at_exit:: count      = 0 ;
        __at_exit __at_exit:: tasks[]    = {};
        bool      __at_exit:: registered = false;
    }
    
    void at_exit:: perform(procedure proc, const longevity when) throw()
    {
        
        
        //______________________________________________________________________
        //
        // sanity check
        //______________________________________________________________________
        assert(proc);
        if(__at_exit::count>=stack_size)
        {
            libc::critical_error(EDOM,"too many at_exit procedures");
        }
        
        //______________________________________________________________________
        //
        // status check
        //______________________________________________________________________
        Y_GIANT_LOCK();
        if(! __at_exit::registered)
        {
            Y_GIANT_LOCK();
            if( 0 != atexit(__at_exit::call) )
            {
                libc::critical_error(errno,"atexit");
            }
            memset( __at_exit::tasks, 0, sizeof(__at_exit::tasks) );
            __at_exit::registered = true;
        }
        
        //______________________________________________________________________
        //
        // create a task at last position
        //______________________________________________________________________
        __at_exit &cb = __at_exit::tasks[ __at_exit::count++ ];
        cb.proc = proc;
        cb.when = when;
        
        //______________________________________________________________________
        //
        // put it into place
        //______________________________________________________________________
        __at_exit::check();
    }
    
}
