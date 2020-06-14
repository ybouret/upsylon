//! \file

#ifndef Y_TYPE_POINTS_INCLUDED
#define Y_TYPE_POINTS_INCLUDED 1

#include "y/type/point3d.hpp"

namespace upsylon {
 
    //! point operations
    struct __point
    {
        //! norm2 in 1D
        template <typename T> static inline
        T norm2(const T a) throw() { return a*a; }
        
        //! norm2 in 2D
        template <typename T> static inline
        T norm2(const point2d<T> &a) throw() { return a.norm2(); }
        
        //! norm2 in 3D
        template <typename T> static inline
        T norm2(const point3d<T> &a) throw() { return a.norm2(); }
        
    };
    
}

#endif

