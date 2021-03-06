//! \file
#ifndef Y_POINT2D_INCLUDED
#define Y_POINT2D_INCLUDED 1

#include "y/mkl/types.hpp"
#include "y/core/isqrt.hpp"

namespace upsylon
{
    //! 2D vector ops
    template <typename T>
    class point2d
    {
    public:
        static const size_t DIMS=2; //!< alias
        Y_DECL_ARGS(T,type);        //!< alias
        
        type x; //!< x coordinate
        type y; //!< y coordinate

        //! output
        friend inline std::ostream & operator<< ( std::ostream &os, const point2d &p )
        {
            return (os << '[' << p.x << ';' << p.y << ']');
        }
        
        //! zero
        inline point2d(): x(0), y(0) {}
        //! by 1 value
        inline point2d( param_type X ) throw() : x(X), y(0) {}
        //! by 2 values
        inline point2d( param_type X, param_type Y) throw() : x(X), y(Y) {}
        //! copy
        inline point2d( const point2d &p ) : x(p.x), y(p.y) {}
        //! construct a vector
        inline point2d( const point2d &A, const point2d &B ) throw() : x(B.x-A.x), y(B.y-A.y) {}
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
        inline point2d   operator-() const { return point2d(-x,-y); }
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
        //! norm1
        inline type norm1() const { return abs_of(x)+abs_of(y); }

        //! in place division by scalar
        inline point2d & operator/=( param_type u) { x/=u;y/=u; return *this; }
        //! div by scalar
        inline friend point2d operator/(const point2d p,param_type u) { return point2d(p.x/u,p.y/u); }

        //! increase all by 1
        inline point2d & inc() throw() { ++x; ++y; return *this; }

        //! increase all by 2
        inline point2d & dec() throw() { --x; --y; return *this; }

        //! swap x and y coordinates
        inline point2d & swap_xy() throw() { bswap(x,y); return *this; }

        //! direct normal
        inline point2d   direct_normal() const throw()
        {
            return point2d(-y,x);
        }

        //! x*y=n, x<=y, starting from x=isqrt(n);
        inline static point2d sqrt_of( param_type n ) throw()
        {
            point2d s;
            if(n>0)
            {
                s.x = isqrt::of<mutable_type>(n); assert(s.x>0);
                s.y = n/s.x;
                while(s.x*s.y!=n)
                {
                    --s.x;
                    s.y=n/s.x;
                }
            }
            return s;
        }

        //! product of coordinates
        inline type prod() const throw()
        {
            return x*y;
        }

        //! sum of coordinates
        inline type sum() const throw()
        {
            return x+y;
        }
        
        //! equality
        inline friend bool operator==( const point2d &lhs, const point2d &rhs )
        {
            return (lhs.x==rhs.x) && (lhs.y==rhs.y);
        }

        //! different
        inline friend bool operator!=( const point2d &lhs, const point2d &rhs )
        {
            return (lhs.x!=rhs.x) || (lhs.y!=rhs.y);
        }

        //! min of coordinates
        static inline point2d min_of( point2d lhs, const point2d rhs ) throw()
        {
            if(rhs.x<lhs.x) lhs.x = rhs.x;
            if(rhs.y<lhs.y) lhs.y = rhs.y;
            return lhs;
        }

        //! min of coordinates
        static inline point2d max_of( point2d lhs, const point2d rhs ) throw()
        {
            if(rhs.x>lhs.x) lhs.x = rhs.x;
            if(rhs.y>lhs.y) lhs.y = rhs.y;
            return lhs;
        }

        //! determinant
        static inline type det( const point2d &lhs, const point2d &rhs ) throw()
        {
            return lhs.x*rhs.y-lhs.y*rhs.x;
        }


        //! dimension as size
        inline size_t size() const throw() { return DIMS; }

        //! access as a C++ array
        inline type & operator[](size_t index) throw()
        {
            assert(index>=1); assert(index<=DIMS);
            return *(((&x)-1) + index);
        }

        //! const access as a C++ array
        inline const_type & operator[](size_t index) const throw()
        {
            assert(index>=1); assert(index<=DIMS);
            return *(((&x)-1) + index);
        }

        //! transform the 2D point with a matrix-like object
        template <typename MATRIX>
        inline point2d mul_by( const MATRIX &M ) const
        {
            return point2d(x * static_cast<const_type>(M[1][1]) + y * static_cast<const_type>(M[1][2]),
                           x * static_cast<const_type>(M[2][1]) + y * static_cast<const_type>(M[2][2]));
        }

        //! foreign points ops
        template <typename U>
        inline point2d( const point2d<U> &p, type2type<U> ) throw() :
        x( type(p.x) ),
        y( type(p.y) )
        {
        }
    };

}

#endif


