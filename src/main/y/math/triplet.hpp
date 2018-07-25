//! \file
#ifndef Y_MATH_TRIPLET_INCLUDED
#define Y_MATH_TRIPLET_INCLUDED 1

#include "y/sort/nwsrt.hpp"
#include <iostream>

namespace upsylon
{
    namespace math
    {
        template <typename T>
        struct triplet
        {
            T a;
            T b;
            T c;
            inline void sort() throw() { nwsrt::on3( &a ); }
            template <typename U>
            inline void co_sort( triplet<U> &other ) throw()
            {
                nwsrt::on3( &a, &other.a );
            }
            inline void ld(const T x) throw() { a=b=c=x; }
            
            friend inline std::ostream & operator<<( std::ostream &os, const triplet x)
            {
                return (os << '[' << x.a << ' ' << x.b << ' ' << x.c << ']' << '\'' );
            }
        };
    }
}

#endif

