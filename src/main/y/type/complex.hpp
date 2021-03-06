//! \file
#ifndef Y_TYPE_COMPLEX_INCLUDED
#define Y_TYPE_COMPLEX_INCLUDED 1

#include "y/os/oor.hpp"
#include <iostream>

namespace upsylon
{

    namespace mkl
    {
        double atan2_of(double,double) throw(); //!< forward
        float  atan2_of(float,float)   throw(); //!< forward
    }

    
    //! complex numbers
    template <typename T>
    class complex
    {
    public:
        typedef T real_type; //!< for templated operation

        T re; //!< real part
        T im; //!< imaginary part

        //______________________________________________________________________
        //
        // settings
        //______________________________________________________________________
        inline  complex() throw() : re(0), im(0) {}                                                //!< default constructor
        inline ~complex() throw() {}                                                               //!< default destructor
        inline  complex(const complex &c) throw() : re(c.re), im(c.im) {}                          //!< just copy
        inline  complex & operator=( const complex c ) throw() { re=c.re; im=c.im; return *this; } //!< just assign
        inline  complex(const real_type x) throw() : re(x), im(0) {}                               //!< build with real part
        inline  complex(const real_type x, const real_type y) throw() : re(x), im(y) {}            //!< build with both parts
        inline  complex & operator=( const real_type x ) throw() { re=x; im=0; return *this; }     //!< assign real part

        //! output
        inline friend std::ostream & operator<<(std::ostream &os, const complex c)
        {
            return (os << '(' << c.re << ',' << c.im << ')');
        }

        //! produce conjugate
        inline complex conj() const throw()
        {
            return complex(re,-im);
        }

        //______________________________________________________________________
        //
        // addition
        //______________________________________________________________________

        //! in place addition
        inline complex & operator+=(const complex c) throw()
        {
            re += c.re;
            im += c.im;
            return *this;
        }

        //! in place addition, scalar
        inline complex & operator+=(const real_type x) throw()
        {
            re += x;
            return *this;
        }

        //! unary plus
        inline complex operator+() const throw() { return *this; }

        //! binary add
        inline friend complex operator+(const complex lhs, const complex rhs) throw()
        {
            return complex(lhs.re+rhs.re,lhs.im+rhs.im);
        }

        //! binary add, scalar lhs
        inline friend complex operator+(const real_type lhs, const complex rhs) throw()
        {
            return complex(lhs+rhs.re,rhs.im);
        }

        //! binary add, scalar rhs
        inline friend complex operator+( const complex lhs, const real_type rhs ) throw()
        {
            return complex(lhs.re+rhs,lhs.im);
        }


        //______________________________________________________________________
        //
        // subtraction
        //______________________________________________________________________

        //! in place sub
        inline complex & operator-=(const complex c) throw()
        {
            re -= c.re;
            im -= c.im;
            return *this;
        }

        //! in place sub, scalar
        inline complex & operator-=(const real_type x) throw()
        {
            re -= x;
            return *this;
        }

        //! unary minus
        inline complex operator-() const throw() { return complex(-re,-im); }

        //! binary sub
        inline friend complex operator-(const complex lhs, const complex rhs)  throw()
        {
            return complex(lhs.re-rhs.re,lhs.im-rhs.im);
        }

        //! binary sub, scalar lhs
        inline friend complex operator-(const real_type lhs, const complex rhs) throw()
        {
            return complex(lhs-rhs.re,-rhs.im);
        }

        //! binary sub, scalar rhs
        inline friend complex operator-(const complex lhs, const real_type rhs ) throw()
        {
            return complex(lhs.re-rhs,lhs.im);
        }

        //______________________________________________________________________
        //
        // multiplication
        //______________________________________________________________________
        
        //! in place mul
        inline complex & operator*=(const complex rhs) throw()
        {
            const complex lhs(*this);
            re = lhs.re * rhs.re - lhs.im * rhs.im;
            im = lhs.re * rhs.im + lhs.im * rhs.re;
            return *this;
        }

        //! in place mul, scalar
        inline complex & operator*=(const real_type x) throw()
        {
            re *= x;
            im *= x;
            return *this;
        }

        //! binary mul
        inline friend complex operator*(const complex lhs, const complex rhs) throw()
        {
            return complex(lhs.re*rhs.re-lhs.im*rhs.im,lhs.re*rhs.im+lhs.im*rhs.re);
        }

        //! binary mul
        inline friend complex operator*(const complex lhs, const real_type x) throw()
        {
            return complex(lhs.re*x,lhs.im*x);
        }

        //! binary mul
        inline friend complex operator*(const real_type x, const complex rhs) throw()
        {
            return complex(x*rhs.re,x*rhs.im);
        }

        //! in place square
        inline void __square() throw()
        {
            const complex   _      = *this;
            const real_type i_half = _.re*_.im;
            const real_type r_full = _.re*_.re - _.im*_.im;
            im = i_half+i_half;
            re = r_full;
        }

        //______________________________________________________________________
        //
        // division
        //______________________________________________________________________

        //! squared modulus
        inline real_type mod2() const throw() { return re*re+im*im; }
        
        //! in place div
        inline complex & operator/=(const complex c) throw()
        {
            const real_type  den = c.mod2();          assert(den>0);
            const complex    num = (*this) * c.conj();
            re = num.re/den;
            im = num.im/den;
            return *this;
        }

        //! in place div, scalar
        inline complex & operator/=(const real_type x) throw()
        {
            assert(x<0||x>0);
            re/=x;
            im/=x;
            return *this;
        }

        //! binary div
        inline friend complex operator/(const complex lhs, const complex rhs) throw()
        {
            const complex   num = lhs * rhs.conj();
            const real_type den = rhs.mod2();
            return complex(num.re/den,num.im/den);
        }

        //! binary div, scalar
        inline friend complex operator/(const complex lhs, const real_type x) throw()
        {
            assert(x<0||x>0);
            return complex(lhs.re/x,lhs.im/x);
        }

        //! binary div, scalar
        inline friend complex operator/(const real_type x, const complex rhs) throw()
        {
            const complex lhs(x);
            return lhs/rhs;
        }

        //______________________________________________________________________
        //
        // other methods
        //______________________________________________________________________

        //! default equality testing
        inline friend bool operator==( const complex &lhs, const complex &rhs ) throw()
        {
            return (lhs.re==rhs.re) && (lhs.im==rhs.im);
        }

        //! default difference testing
        inline friend bool operator!=( const complex &lhs, const complex &rhs ) throw()
        {
            return (lhs.re!=rhs.re) || (lhs.im!=rhs.im);
        }

        //! argument
        inline T arg() throw()
        {
            return mkl::atan2_of(im,re);
        }


        inline T       * re_addr()       throw() { return &re; }  //!< &re
        inline const T * re_addr() const throw() { return &re; }  //!< &re

        inline T       *re_prev()       throw() { return out_of_reach::prev(&re); } //!< (&re)-1
        inline const T *re_prev() const throw() { return out_of_reach::prev(&re); } //!< (&re)-1

    };


}

#endif
