// \file
#ifndef Y_MEMORY_IO_INCLUDED
#define Y_MEMORY_IO_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace memory
    {
        struct io
        {
            template <typename T>
            static T *cast( void *addr, const ptrdiff_t shift=0) throw()
            {
                char *p=static_cast<char *>(addr);
                void *q=static_cast<void *>(p+shift);
                return static_cast<T*>(q);
            }
            template <typename T>
            static ptrdiff_t delta(const T *a, const T *b) throw()
            {
                return static_cast<ptrdiff_t>(b-a)*ptrdiff_t(sizeof(T));
            }
        };
    }
}

#endif

