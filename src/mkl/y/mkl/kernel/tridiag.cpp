#include "y/mkl/kernel/tridiag.hpp"


namespace upsylon {

    namespace mkl {

        tridiag_info:: ~tridiag_info() throw()
        {
        }

        tridiag_info:: tridiag_info(const size_t x) throw() :
        extraneous(x)
        {
        }

    }
}
