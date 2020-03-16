
//! \file

#ifndef Y_COUNTING_PERM_OPS_INCLUDED
#define Y_COUNTING_PERM_OPS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace core {

        //! basic function for permutation
        struct permutation
        {
            static bool next(size_t *p, const size_t n) throw(); //!< compute next permutation, without checking
            static void _nxt(size_t *p, const size_t n) throw(); //!< compute next permutation, with checking
        };
    }

}

#endif

