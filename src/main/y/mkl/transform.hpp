//! \file

#ifndef Y_MKL_TRANSFORM_INCLUDED
#define Y_MKL_TRANSFORM_INCLUDED 1

#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace mkl
    {

        template <typename T>
        struct transform
        {
            Y_DECL_ARGS(T,type);
            typedef typename real_for<mutable_type>::type real_t;
            static inline type   Id(const_type &x)   { return x;          }
            static inline type   Abs(const_type &x)  { return fabs_of(x); }
            static inline real_t Mod2(const_type &x) { return mod2_of(x); }
        };

    }
}

#endif

