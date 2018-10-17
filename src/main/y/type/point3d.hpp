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

        //! output
        friend inline std::ostream & operator<< ( std::ostream &os, const point3d &p )
        {
            return (os << '[' << p.x << ' ' << p.y << ' ' << p.z << ']' << '\'');
        }

        //! default ctor, zero
        inline point3d() : x(0), y(0), z(0) {}
        //! construct by values
        inline point3d( param_type X, param_type Y, param_type Z) : x(X), y(Y), z(Z) {}
        //! copy constructor
        inline point3d( const point3d &p ) : x(p.x), y(p.y), z(p.z) {}
        //! assign
        inline point3d  & operator=( const point3d p ) { x=p.x;y=p.y;z=p.z;return *this; }
        //! in place addition
        inline point3d & operator+=( const point3d p ) { x+=p.x; y+=p.y; z+=p.z; return *this; }
        //! unary +
        inline point3d   operator+() { return point3d(x,y,z); }
        //! addition
        inline friend point3d   operator+(const point3d lhs, const point3d rhs) { return point3d(lhs.x+rhs.x,lhs.y+rhs.y,lhs.z+rhs.z); }

        //! in place sub
        inline point3d & operator-=( const point3d p ) { x-=p.x; y-=p.y; z-=p.z; return *this; }
        //! unary -
        inline point3d   operator-() { return point3d(-x,-y,-z); }
        //! sub
        inline friend point3d   operator-(const point3d lhs, const point3d rhs) { return point3d(lhs.x-rhs.x,lhs.y-rhs.y,lhs.z-rhs.z); }

        //! in place mul by scalar
        inline point3d & operator*=( param_type u ) { x*=u;y*=u;z*=u;return *this; }
        //! mul by scalar
        inline friend point3d operator*(param_type u,const point3d p) { return point3d(p.x*u,p.y*u,p.z*u); }
        //! mul by scalar
        inline friend point3d operator*(const point3d p,param_type u) { return point3d(p.x*u,p.y*u,p.z*u); }
        //! dot product
        inline friend type operator*(const point3d a, const point3d b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
        //! norm2
        inline type norm2() const { return x*x+y*y+z*z; }
        //! in place division by scalar
        inline point3d & operator/=( param_type u) { x/=u;y/=u;z/=u; return *this; }
        //! div by scalar
        inline friend point3d operator/(const point3d p,param_type u) { return point3d(p.x/u,p.y/u,p.z/u); }

        //! cross product
        inline static point3d cross( const point3d a, const point3d b )
        {
            return point3d(a.y*b.z-a.z*b.y,b.x*a.z-a.x*b.z,a.x*b.y-a.y*b.x);
        }

        //! change semantics
        inline friend point3d operator^(const point3d a, const point3d b) { return cross(a,b); }

        //! increase all by 1
        inline point3d & inc() throw() { ++x; ++y; ++z; return *this; }
        //! decrease all by 1
        inline point3d & dec() throw() { --x; --y; --z; return *this; }

    };
}

#endif

