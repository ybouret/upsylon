#include "y/memory/global.hpp"
#include "y/exceptions.hpp"
#include <cstdlib>
#include <cerrno>
#include <iostream>

namespace upsylon
{

    namespace  memory
    {

        global:: ~global() throw()
        {
            if(allocated!=0)
            {
                std::cerr << "[memory.global] allocated=" << allocated << std::endl;
            }
        }


        global:: global() throw() : allocated(0)
        {
        }


        void * global:: acquire(size_t &n)
        {
            Y_LOCK(access);
            if(n>0)
            {
                void *p = calloc(1,n);
                if(!p)
                {
                    const unsigned long long desired = n;
                    n = 0;
                    throw libc::exception(ENOMEM,"memory.global.acquire(%llu)",desired);
                }
                allocated += n;
                return p;
            }
            else
            {
                return 0;
            }
        }

        void global:: release(void *&p, size_t &n) throw()
        {
            Y_LOCK(access);
            if(p)
            {
                assert(n>0);
                free(p);
                allocated -= int64_t(n);
                n = 0;
                p = NULL;
            }
            else
            {
                assert(0==n);
            }
        }

        void *global:: __calloc(const size_t count, const size_t size)
        {
            Y_LOCK(access);
            assert(size>0);
            if(count>0)
            {
                void *p = calloc(count,size);
                if(!p) throw libc::exception(ENOMEM,"calloc(%u,%u)",unsigned(count),unsigned(size));
                allocated += int64_t(count*size);
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
                allocated -= int64_t(bytes);
            }
            else
            {
                assert(0==p);
            }
        }

    }
}

