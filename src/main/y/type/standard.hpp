//!\file
#ifndef Y_TYPE_DIVS_INCLUDED
#define Y_TYPE_DIVS_INCLUDED 1

#include "y/type/ints.hpp"
#include <cstdlib>

namespace upsylon {

    template <typename T> struct standard;

    template <> struct standard<int>
    {
        typedef div_t  div_type;
        static  inline div_type div_call(const int num, const int den) { return div(num,den); }
    };

    template <> struct standard<long>
    {
        typedef ldiv_t  div_type;
        static  inline  div_type div_call(const long num, const long den) { return ldiv(num,den); }
    };

    template <> struct standard<long long>
    {
        typedef lldiv_t  div_type;
        static  inline   div_type div_call(const long long num, const long long den) { return lldiv(num,den); }
    };
    
}

#endif

