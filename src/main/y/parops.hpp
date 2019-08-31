//! \file
#ifndef Y_PAROPS_INCLUDED
#define Y_PAROPS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! common parallel operations
    struct parops
    {
        template <typename T, typename U>  static inline
        void split_any( T &length, T &offset, const U Size, const U Rank ) throw()
        {
            assert(Size>0);
            assert(Rank>=0);
            assert(Rank<Size);
            T todo(length/Size);
            for(U r=1;r<=Rank;++r)
            {
                length -= todo;
                offset += todo;
                todo    = length/(Size-r);
            }
            length = todo;
        }
    };
}

#endif

