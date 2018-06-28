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
            Y_LOCK(access);
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
            Y_LOCK(access);
            assert(p);
            assert(n>0);
            free(p);
            n=0;
            p=0;
        }

        void *global:: __calloc(const size_t count, const size_t size)
        {
            Y_LOCK(access);
            assert(size>0);
            if(count>0)
            {
                void *p = calloc(count,size);
                if(!p) throw libc::exception(ENOMEM,"calloc(%u,%u)",unsigned(count),unsigned(size));
                (int64_t &)allocated += (count*size);
                return p;
            }
            else
            {
                return 0;
            }
        }

        void global:: __free(void *p,const size_t bytes) throw()
        {
            Y_LOCK(access);
            if(bytes)
            {
                assert(p);
                free(p);
                (int64_t &)allocated -= bytes;
            }
            else
            {
                assert(0==p);
            }
        }

    }
}

