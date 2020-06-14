//! \file

#ifndef Y_TYPE_POINTS_INCLUDED
#define Y_TYPE_POINTS_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon {
 
    //! point operations
    struct __point
    {
        template <typename T> static inline
        T norm2(const T a) throw() { return a*a; }
        
        template <typename T> static inline
        T norm2(const point2d<T> &a) throw() { return a.norm2(); }
        
        template <typename T> static inline
        T norm2(const point3d<T> &a) throw() { return a.norm2(); }
        
    };
    
}

#endif

