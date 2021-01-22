
#include "y/mkl/solve/zseek.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace mkl {

        zseek:: ~zseek() throw()
        {
        }

        zseek:: zseek() throw()
        {
        }

        void zseek:: error_not_bracketed() const
        {
            throw libc::exception( EDOM, "%s(zero is not bracketed)", method() );
        }

    }

}

