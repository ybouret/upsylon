
//! \file
#ifndef Y_TYPE_SELF_DESTRUCT_INCLUDED
#define Y_TYPE_SELF_DESTRUCT_INCLUDED 1

#include "y/type/traits.hpp"

namespace upsylon
{

    //! in place destructor
    template <typename T>
    inline void self_destruct( T &data ) throw()
    {
        typedef typename type_traits<T>::mutable_type mutable_T;
        mutable_T *item = (mutable_T *)&data;
        item->~mutable_T();
    }
}

#endif

