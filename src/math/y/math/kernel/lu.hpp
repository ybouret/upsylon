//! \file
#ifndef Y_MATH_LU_INCLUDED
#define Y_MATH_LU_INCLUDED 1

#include "y/math/types.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace math
    {

        struct LU
        {
            template <typename T>
            bool build( matrix<T> &a, bool *dneg=0 ) throw()
            {
                assert(a.rows>0);
                assert(a.is_square);
                if(dneg) *dneg=false;
                return true;
            }
        };

    }
}

#endif
