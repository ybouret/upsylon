
#include "y/memory/allocator.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"
#include <iostream>
#include <cerrno>

namespace upsylon
{
    namespace memory
    {
        allocator:: ~allocator() throw()
        {
            if(allocated!=0)
            {
                std::cerr << "[memory] allocated=" << allocated << std::endl;
            }
        }
        
        allocator:: allocator() throw() : allocated(0)
        {
        }
        
        void * allocator:: acquire(size_t &n)
        {
            if(n>0)
            {
                try
                {
                    void *p = __acquire(n);
                    if(!p)
                    {
                        throw libc::exception(ENOMEM,"internal memory failure");
                    }
                    if(n<=0)
                    {
                        throw libc::exception(EINVAL,"valid address with zero length!");
                    }
                    (int64_t &)allocated += n;
                    return p;
                }
                catch(...)
                {
                    n = 0;
                    throw;
                }
            }
            else
            {
                return 0;
            }
        }
        
        void allocator:: release(void *&p, size_t &n) throw()
        {
            if(p)
            {
                assert(n>0);
                (int64_t&)allocated -= n;
                __release(p,n);
            }
            else
            {
                assert(n<=0);
            }
        }
    }
    
}

