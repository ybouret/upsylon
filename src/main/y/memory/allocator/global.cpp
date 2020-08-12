#include "y/memory/allocator/global.hpp"
#include "y/exceptions.hpp"
#include "y/os/run-time-log.hpp"
#include <cstdlib>
#include <cerrno>

namespace upsylon
{

    Y_SINGLETON_IMPL(memory::global);

    namespace  memory
    {

        //const char global::call_sign[] = "memory::global";

        global:: ~global() throw()
        {
            if(allocated!=0)
            {
                rtl(run_time_warning) << uuid << " still allocated=" << allocated << std::endl;
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
                    const unsigned long desired = (unsigned long)n;
                    n = 0;
                    throw libc::exception(ENOMEM,"%s acquire(%lu)",uuid,desired);
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
#if !defined(NDEBUG)
            else
            {
                assert(0==n);
            }
#endif
        }

        void *global:: __calloc(const size_t count, const size_t size)
        {
            Y_LOCK(access);
            assert(size>0);
            if(count>0)
            {
                void *p = calloc(count,size);
                if(!p) throw libc::exception(ENOMEM,"%s calloc(%u,%u)",uuid,unsigned(count),unsigned(size));
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
#if !defined(NDEBUG)
            else
            {
                assert(0==p);
            }
#endif
        }

    }
}

