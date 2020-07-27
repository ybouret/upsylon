#include "y/mkl/fcn/zfind.hpp"
#include "y/exceptions.hpp"

#include <cerrno>

namespace upsylon {

    namespace mkl {
        
        const unsigned zfind::__z;
        const unsigned zfind::__n;
        const unsigned zfind::__p;

        const char *zfind:: sign_text(const unsigned s) throw()
        {
            switch(s)
            {
                case __z: return "[  zero  ]";
                case __p: return "[positive]";
                case __n: return "[negative]";
                default: break;
            }

            return "unknown";
        }
        
        void zfind:: throw_not_bracketed()
        {
             throw libc::exception(EDOM,"zfind: zero is not bracketed");
        }
    }
}

