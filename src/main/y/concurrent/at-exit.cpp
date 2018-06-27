#include "y/concurrent/at-exit.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/os/error.hpp"
#include "y/type/bswap.hpp"
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
            void               *args;
            at_exit::longevity  when;

            static __at_exit tasks[at_exit::stack_size];
            static size_t    count;

            static inline void call() throw()
            {
                for(size_t i=0;i<count;++i)
                {
                    __at_exit &cb = tasks[i];
                    assert(cb.proc);
                    try{cb.proc(cb.args);} catch(...) {}
                    memset(&cb,0,sizeof(__at_exit));
                }
            }

            static inline void check() throw()
            {
                __at_exit *curr = tasks + count;
                __at_exit *prev = curr-1;
                --curr;
                --prev;
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

        };

        size_t    __at_exit:: count   = 0 ;
        __at_exit __at_exit:: tasks[] = {};

    }

    void at_exit:: perform(procedure proc, void *args, const longevity when) throw()
    {
        static bool registerd = false;

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
        if(!registerd)
        {
            Y_GIANT_LOCK();
            if( 0 != atexit(__at_exit::call) )
            {
                libc::critical_error(errno,"atexit");
            }
            registerd = true;
        }

        // create a task at last position
        __at_exit &cb = __at_exit::tasks[ __at_exit::count++ ];
        cb.proc = proc;
        cb.args = args;
        cb.when = when;

        // put it into place
        __at_exit::check();
    }

}
