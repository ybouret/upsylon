

//! \file
#ifndef Y_TYPE_COLLAPSE_INCLUDED
#define Y_TYPE_COLLAPSE_INCLUDED 1

#include "y/type/traits.hpp"
//#include "y/os/oor.hpp"

namespace upsylon
{
    
    //! in place destructor
    template <typename T>
    inline void collapse(T &data) throw()
    {
        typedef typename type_traits<T>::mutable_type mutable_T;
        mutable_T *item = (mutable_T *)&data;
        item->~mutable_T();
    }
    
    
    //! in place destructor
    template <typename T>
    inline T *collapsed(T *addr) throw()
    {
        collapse<T>(*addr);
        return addr;
    }
    
    
    
}

#endif

