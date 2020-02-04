//!\file
#ifndef Y_TYPE_DIVS_INCLUDED
#define Y_TYPE_DIVS_INCLUDED 1

#include "y/type/ints.hpp"
#include <cstdlib>

namespace upsylon {

    namespace core
    {
        template <typename T> struct div_of;

        template <>
        struct div_of<int>
        {
            typedef div_t  type;
            static inline type _(const int num, const int den)
            {
                return div(num,den);
            }
        };

        template <>
        struct div_of<long>
        {
            typedef ldiv_t type;
            static inline  type _(const int num, const int den)
            {
                return ldiv(num,den);
            }
        };

        template <>
        struct div_of<long long>
        {
            typedef lldiv_t type;
            static inline   type _(const long long num, const long long den)
            {
                return lldiv(num,den);
            }
        };
    }
}

#endif

