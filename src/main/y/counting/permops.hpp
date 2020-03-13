
//! \file

#ifndef Y_COUNTING_PERMOPS_INCLUDED
#define Y_COUNTING_PERMOPS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    struct permops
    {
        static void init(size_t *p, const size_t n) throw();            //!< p[1..n]=1..n
        static void init(size_t *p, const size_t n, size_t *a) throw(); //!< p[1..n]=1..n,a[0..n-1]=0..n-1
        static bool next(size_t *p, const size_t n) throw();            //!< compute next permutation
        static void to_C(size_t *a, const size_t *p, const size_t n) throw(); //!< a[0..n-1] = p[1..n]-1
    };

}

#endif

