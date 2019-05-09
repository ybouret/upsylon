
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

        }
        
        allocator:: allocator() throw() 
        {
        }
        
        uint8_t * allocator:: acquire_bytes(size_t &n )
        {
            return static_cast<uint8_t *>( acquire(n) );
        }

        void allocator:: release_bytes(uint8_t * &p, size_t &n) throw()
        {
            void *q = static_cast<void *>(p);
            release(q,n); assert(0==q);
            p = 0;
        }

    }
    
}

