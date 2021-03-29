//! \file
#ifndef Y_PAROPS_INCLUDED
#define Y_PAROPS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! common parallel operations
    //
    //__________________________________________________________________________
    struct parops
    {
        //! get local task from global info
        template <typename T, typename U>  static inline
        void split_any(T &length, T &offset, const U Size, const U Rank) throw()
        {
            assert(Size>0);
            assert(Rank>=0);
            assert(Rank<Size);
            T todo = T(U(length)/Size);
            for(U r=1;r<=Rank;++r)
            {
                length -= todo;
                offset += todo;
                todo    = T( U(length)/(Size-r) );
            }
            length = todo;
        }
        
        //! get the next rank in 1D, MPI style
        template <typename T> static inline
        T rank_next(T size, T rank) throw()
        {
            assert(size>0); assert(rank>=0); assert(rank<size);
            return (++rank>=size) ? 0 : rank;
        }
        
        //! get the previous rank in 1D, MPI style
        template <typename T> static inline
        T rank_prev(T size, T rank) throw()
        {
            assert(size>0); assert(rank>=0); assert(rank<size);
            return (rank<=0) ? --size : --rank;
        }
        
        
    };
}

#endif

