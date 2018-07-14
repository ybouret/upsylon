//! \file
#ifndef Y_TYPE_BMOVE_INCLUDED
#define Y_TYPE_BMOVE_INCLUDED 1

#include "y/os/platform.hpp"
#include <cstring>

namespace upsylon
{
    namespace core
    {
        //! block move, default
        template <const size_t N>
        inline void bmove(void *target,const void *source) throw()
        {
            memmove(target,source,N);
        }
    }

    //! inline block move of a given type
    template <typename T>
    inline void bmove( T &target, const T &source) throw()
    {
        core::bmove<sizeof(T)>( &target, &source);
    }
}

#endif

