//! \file
#ifndef Y_MKL_TRIPLET_INCLUDED
#define Y_MKL_TRIPLET_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/utils.hpp"
#include "y/sort/network/sort3.hpp"
#include "y/comparison.hpp"
#include <iostream>

namespace upsylon {

    namespace mkl {

        //______________________________________________________________________
        //
        //! triplet of POD to bracket values
        //
        //______________________________________________________________________
        template <typename T>
        struct triplet
        {
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);           //!< aliases
            typedef network_sort<3> nwsrt; //!< for [co]sorting data

            //__________________________________________________________________
            //
            // the three values
            //__________________________________________________________________

            T a; //!< first
            T b; //!< second
            T c; //!< third

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! inline sorting
            inline void sort() throw()
            {
                nwsrt::on( &a, comparison::increasing<T> );
            }

            //! inline co-sorting
            template <typename U>
            inline void co_sort( triplet<U> &other ) throw()
            {
                nwsrt::co( &a, &other.a, comparison::increasing<T> );
            }

            //! set same value
            inline void ld(param_type x) throw() { a=b=c=x; }

            //! set other triplet
            inline void ld(const triplet &other) throw()
            {
                a=other.a;
                b=other.b;
                c=other.c;
            }

            //! add
            inline void add(const triplet &other) throw()
            {
                a+=other.a;
                b+=other.b;
                c+=other.c;
            }

            //! sub
            inline void sub(const triplet &other) throw()
            {
                a-=other.a;
                b-=other.b;
                c-=other.c;
            }
            
            //! output
            friend inline std::ostream & operator<<( std::ostream &os, const triplet x)
            {
                return (os << '[' << x.a << ' ' << x.b << ' ' << x.c << ']' << '\'' );
            }

            //! check increasing order
            inline bool is_increasing() const throw()
            {
                return (a<=b) && (b<=c);
            }

            //! check decreasing order
            inline bool is_decreasing() const throw()
            {
                return (c<=b) && (b<=a);
            }

            //! check ordered
            inline bool is_ordered() const throw()
            {
                return is_increasing() || is_decreasing();
            }

            //! shift value a <- b <- c <- d
            inline void shift(const T d) throw() { a=b; b=c; c=d; }

            //! for quark, if necessary
            inline size_t size() const throw() { return 3; }

            //! for access
            inline type & operator[](size_t indx) throw()
            { assert(indx>=1); assert(indx<=3); return * ( (&a) + --indx ); }

            //! for access
            inline const_type & operator[](size_t indx) const throw()
            { assert(indx>=1); assert(indx<=3); return * ( (&a) + --indx ); }

            //! no-throw swap
            inline void swap_with( triplet &other )  throw()
            {
                bswap(a,other.a);
                bswap(b,other.b);
                bswap(c,other.c);
            }

            //! a^2+b^2+c^2
            inline type norm2() const throw()
            {
                return square_of(a) + square_of(b) + square_of(c);
            }

            //! squared distance
            inline type diff2(const triplet &other) const throw()
            {
                return square_of(a-other.a) + square_of(b-other.b) + square_of(c-other.c);
            }
        };
    }
}

#endif

