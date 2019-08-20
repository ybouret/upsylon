//! \file
#ifndef Y_ITERATE_FOR_EACH_INCLUDED
#define Y_ITERATE_FOR_EACH_INCLUDED 1

#include "y/core/loop.hpp"

namespace upsylon
{

    template <typename ITERATOR, typename FUNC>
    void for_each(const size_t n, ITERATOR iter, FUNC &func )
    {
        Y_LOOP(n,func(*iter);++iter);
    }

    template <typename SEQUENCE, typename FUNC>
    void for_each( SEQUENCE &seq, FUNC &func )
    {
        for_each( seq.size(), seq.begin(), func);
    }



    template <typename ITERATOR, typename FUNC>
    void for_each_meta(const size_t n, ITERATOR iter, FUNC &func )
    {
        Y_LOOP(n,func(**iter);++iter);
    }

    template <typename SEQUENCE, typename FUNC>
    void for_each_meta( SEQUENCE &seq, FUNC &func )
    {
        for_each_meta( seq.size(), seq.begin(), func);
    }

}

#endif

