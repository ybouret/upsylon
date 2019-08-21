//! \file
#ifndef Y_ITERATE_FOR_EACH_INCLUDED
#define Y_ITERATE_FOR_EACH_INCLUDED 1

#include "y/core/loop.hpp"

namespace upsylon
{

    //! iter function on a range
    template <typename ITERATOR, typename FUNC>
    void for_each(const size_t n, ITERATOR iter, FUNC &func )
    {
        Y_LOOP(n,func(*iter);++iter);
    }

    //! iter function on a full sequence
    template <typename SEQUENCE, typename FUNC>
    void for_each( SEQUENCE &seq, FUNC &func )
    {
        for_each( seq.size(), seq.begin(), func);
    }

    //! iter function on a range of pointers
    template <typename ITERATOR, typename FUNC>
    void for_each_meta(const size_t n, ITERATOR iter, FUNC &func )
    {
        Y_LOOP(n,func(**iter);++iter);
    }

    //! iter function on a full sequence of pointers
    template <typename SEQUENCE, typename FUNC>
    void for_each_meta( SEQUENCE &seq, FUNC &func )
    {
        for_each_meta( seq.size(), seq.begin(), func);
    }

}

#endif

