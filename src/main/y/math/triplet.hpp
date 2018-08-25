//! \file
#ifndef Y_MATH_TRIPLET_INCLUDED
#define Y_MATH_TRIPLET_INCLUDED 1

#include "y/sort/nwsrt.hpp"
#include <iostream>

namespace upsylon
{
    namespace math
    {
        //! triplet of POD to bracket values
        template <typename T>
        struct triplet
        {
            T a; //!< first
            T b; //!< second
            T c; //!< third
            //! inline sorting
            inline void sort() throw() { nwsrt::on3( &a ); }
            //! inline co-sorting
            template <typename U>
            inline void co_sort( triplet<U> &other ) throw()
            {
                nwsrt::on3( &a, &other.a );
            }
            //! set same value
            inline void ld(const T x) throw() { a=b=c=x; }

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


        };
    }
}

#endif

