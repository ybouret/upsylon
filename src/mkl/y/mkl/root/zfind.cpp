#include "y/mkl/root/zfind.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace mkl {

        zfind:: ~zfind() throw()
        {
        }

        zfind:: zfind() throw()
        {
        }
        
        void zfind:: error_not_bracketed() const
        {
            throw libc::exception( EDOM, "zfind: zero is not bracketed" );
        }

    }

}

