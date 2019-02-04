#include "y/memory/io.hpp"

namespace upsylon
{
    namespace memory
    {
        void * io::__shift(void *addr, const ptrdiff_t bytes) throw()
        {
            char *p = static_cast<char *>(addr);
            return    static_cast<void *>(p+bytes);
        }

        const void * io::__shift(const void *addr, const ptrdiff_t bytes) throw()
        {
            const char *p = static_cast<const char *>(addr);
            return          static_cast<const void *>(p+bytes);
        }

        void * io::__addr( void *addr ) throw()
        {
            return addr;
        }

        const void * io::__addr( const void *addr ) throw()
        {
            return addr;
        }
        

    }
}
