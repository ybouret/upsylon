#include "y/memory/global.hpp"
#include <cstdlib>
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{

    namespace  memory
    {

        global:: ~global() throw()
        {

        }

        global:: global() throw()
        {
        }


        void * global:: __acquire(size_t &n)
        {
            assert(n>0);
            void *p = calloc(1,n);
            if(!p)
            {
                const unsigned nn = n;
                n=0;
                throw libc::exception(ENOMEM,"calloc(%u)",nn);
            }
            return p;
        }

        void global:: __release(void *&p, size_t &n) throw()
        {
            assert(p);
            assert(n>0);
            free(p);
            n=0;
        }
        

    }
}

