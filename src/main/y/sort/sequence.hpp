//! \file
#ifndef Y_SORT_SEQUENCE_INCLUDED
#define Y_SORT_SEQUENCE_INCLUDED 1

#include "y/sequence/array.hpp"
#include "y/sequence/list.hpp"
#include "y/sort/heap.hpp"

namespace upsylon
{

    //! overloaded sort a sequence based on an array
    template <typename T,typename FUNC>
    inline void sort_sequence( array<T> &seq, FUNC &cmp )
    {
        hsort(seq,cmp);
    }

    //! overloaded sort a sequence based on a list
    template <typename T,typename FUNC>
    inline void sort_sequence( list<T> &seq, FUNC &cmp )
    {
        seq.sort(cmp);
    }

}

#endif

