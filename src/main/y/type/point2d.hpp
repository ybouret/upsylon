//! \file
#ifndef Y_POINT2D_INCLUDED
#define Y_POINT2D_INCLUDED 1

#include "y/math/types.hpp"

namespace upsylon
{
    //! 2D vector ops
    template <typename T>
    class point2d
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias
        type x; //!< x coordinate
        type y; //!< y coordinate

        //! zero
        inline point2d(): x(0), y(0) {}
        //! by values
        inline point2d( param_type X, param_type Y) throw() : x(X), y(Y) {}
        //! copy
        inline point2d( const point2d &p ) : x(p.x), y(p.y) {}
        //! assign
        inline point2d  & operator=( const point2d &p ) { x=p.x;y=p.y;return *this; }

    };

}

#endif


