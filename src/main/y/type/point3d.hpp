//! \file
#ifndef Y_POINT3D_INCLUDED
#define Y_POINT3D_INCLUDED 1

#include "y/type/point2d.hpp"

namespace upsylon
{
    //! 3D vector ops
    template <typename T>
    class point3d
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias
        type x; //!< x coordinate
        type y; //!< y coordinate
        type z; //!< z coordinate

        //! default ctor, zero
        inline point3d() : x(0), y(0), z(0) {}
        //! construct by values
        inline point3d( param_type X, param_type Y, param_type Z) : x(X), y(Y), z(Z) {}
        //! copy constructor
        inline point3d( const point3d &p ) : x(p.x), y(p.y), z(p.z) {}
        //! assign
        inline point3d  & operator=( const point3d &p ) { x=p.x;y=p.y;z=p.z;return *this; }

    };
}

#endif

