
//! \file
#ifndef Y_POINT1D_INCLUDED
#define Y_POINT1D_INCLUDED 1

#include "y/type/args.hpp"
#include <iostream>

namespace upsylon
{
    //! 1D vector ops
    template <typename T>
    class point1d
    {
    public:
        static const size_t DIMS=1; //!< alias
        Y_DECL_ARGS(T,type);        //!< alias
        
        type x; //!< x coordinate
        
        //! output
        friend inline std::ostream & operator<< ( std::ostream &os, const point1d &p )
        {
            return ( os << p.x );
        }
        
        //! access
        inline type & operator*() throw() { return x; }
        //! access
        inline type & operator*() const throw() { return x; }
        //! zero
        inline point1d(): x(0)  {}
        //! by 1 value
        inline point1d( param_type X ) throw() : x(X) {}
        //! copy
        inline point1d( const point1d &p ) : x(p.x) {}
        //! construct a vector
        inline point1d( const point1d &A, const point1d &B ) throw() : x(B.x-A.x) {}
        //! assign
        inline point1d  & operator=( const point1d p ) { x=p.x;return *this; }
        //! in place addition
        inline point1d & operator+=( const point1d p ) { x+=p.x; return *this; }
        //! unary +
        inline point1d   operator+() { return point1d(x); }
        //! addition
        inline friend point1d   operator+(const point1d lhs, const point1d rhs) { return point1d(lhs.x+rhs.x); }
        
        //! in place sub
        inline point1d & operator-=( const point1d p ) { x-=p.x; return *this; }
        //! unary -
        inline point1d   operator-() const { return point1d(-x); }
        //! sub
        inline friend point1d   operator-(const point1d lhs, const point1d rhs) { return point1d(lhs.x-rhs.x); }

        //! in place mul by scalar
        inline point1d & operator*=( param_type u ) { x*=u; return *this; }
        //! mul by scalar
        inline friend point1d operator*(param_type u,const point1d p) { return point1d(p.x*u); }
        //! mul by scalar
        inline friend point1d operator*(const point1d p,param_type u) { return point1d(p.x*u); }
        //! dot product
        inline friend type operator*(const point1d a, const point1d b) { return a.x*b.x; }
        //! norm2
        inline type norm2() const { return x*x; }
        
        //! in place division by scalar
        inline point1d & operator/=( param_type u) { x/=u; return *this; }
        //! div by scalar
        inline friend point1d operator/(const point1d p,param_type u) { return point1d(p.x/u); }
        
        //! increase all by 1
        inline point1d & inc() throw() { ++x; return *this; }
        
        //! increase all by 2
        inline point1d & dec() throw() { --x; return *this; }
        
        
        //! product of coordinates
        inline type prod() const throw() { return x; }
        
        //! sum of coordinates
        inline type sum() const throw() { return x; }
        
        //! equality
        inline friend bool operator==( const point1d &lhs, const point1d &rhs )
        {
            return (lhs.x==rhs.x);
        }
        
        //! different
        inline friend bool operator!=( const point1d &lhs, const point1d &rhs )
        {
            return (lhs.x!=rhs.x);
        }
        
        //! min of coordinates
        static inline point1d min_of( point1d lhs, const point1d rhs ) throw()
        {
            if(rhs.x<lhs.x) lhs.x = rhs.x;
            return lhs;
        }
        
        //! min of coordinates
        static inline point1d max_of( point1d lhs, const point1d rhs ) throw()
        {
            if(rhs.x>lhs.x) lhs.x = rhs.x;
            return lhs;
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
        
        //! foreign points ops
        template <typename U>
        inline point1d( const point1d<U> &p, type2type<U> ) throw() :
        x( type(p.x) )
        {
        }
    };
    
}

#endif


