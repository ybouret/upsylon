

//! \file
#ifndef Y_TYPE_COLLAPSE_INCLUDED
#define Y_TYPE_COLLAPSE_INCLUDED 1

#include "y/type/traits.hpp"
#include "y/os/oor.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //! in place destructor
    //__________________________________________________________________________
    template <typename T>
    inline void collapse(T &data) throw()
    {
        typedef typename type_traits<T>::mutable_type mutable_T;
        mutable_T *item = (mutable_T *)&data;
        item->~mutable_T();
    }

    //__________________________________________________________________________
    //
    //! in place destructor with zeroing
    //__________________________________________________________________________
    template <typename T>
    inline void zcollapse(T &data) throw()
    {
        typedef typename type_traits<T>::mutable_type mutable_T;
        mutable_T *item = (mutable_T *)&data;
        item->~mutable_T();
        out_of_reach::fill(item,0,sizeof(T));
    }


    //__________________________________________________________________________
    //
    //! in place destructor
    //__________________________________________________________________________
    template <typename T>
    inline T *collapsed(T *addr) throw()
    {
        collapse<T>(*addr);
        return addr;
    }

    //__________________________________________________________________________
    //
    //! in place destructor with zeroing
    //__________________________________________________________________________
    template <typename T>
    inline T *zcollapsed(T *addr) throw()
    {
        zcollapse<T>(*addr);
        return addr;
    }
    
    
    
}

#endif

