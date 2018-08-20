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

        //! output
        friend inline std::ostream & operator<< ( std::ostream &os, const point2d &p )
        {
            return (os << '[' << p.x << ' ' << p.y << ']' << '\'');
        }
        
        //! zero
        inline point2d(): x(0), y(0) {}
        //! by values
        inline point2d( param_type X, param_type Y) throw() : x(X), y(Y) {}
        //! copy
        inline point2d( const point2d &p ) : x(p.x), y(p.y) {}
        //! assign
        inline point2d  & operator=( const point2d p ) { x=p.x;y=p.y;return *this; }

        //! in place addition
        inline point2d & operator+=( const point2d p ) { x+=p.x; y+=p.y; return *this; }
        //! unary +
        inline point2d   operator+() { return point2d(x,y); }
        //! addition
        inline friend point2d   operator+(const point2d lhs, const point2d rhs) { return point2d(lhs.x+rhs.x,lhs.y+rhs.y); }

        //! in place sub
        inline point2d & operator-=( const point2d p ) { x-=p.x; y-=p.y; return *this; }
        //! unary -
        inline point2d   operator-() { return point2d(-x,-y); }
        //! sub
        inline friend point2d   operator-(const point2d lhs, const point2d rhs) { return point2d(lhs.x-rhs.x,lhs.y-rhs.y); }

        //! in place mul by scalar
        inline point2d & operator*=( param_type u ) { x*=u;y*=u;return *this; }
        //! mul by scalar
        inline friend point2d operator*(param_type u,const point2d p) { return point2d(p.x*u,p.y*u); }
        //! mul by scalar
        inline friend point2d operator*(const point2d p,param_type u) { return point2d(p.x*u,p.y*u); }
        //! dot product
        inline friend type operator*(const point2d a, const point2d b) { return a.x*b.x + a.y*b.y; }
        //! norm2
        inline type norm2() const { return x*x+y*y; }

        //! in place division by scalar
        inline point2d & operator/=( param_type u) { x/=u;y/=u; return *this; }
        //! div by scalar
        inline friend point2d operator/(const point2d p,param_type u) { return point2d(p.x/u,p.y/u); }

        //! increase all by 1
        inline point2d & inc() throw() { ++x; ++y; return *this; }

        //! increase all by 2
        inline point2d & dec() throw() { --x; --y; return *this; }
    };

}

#endif


