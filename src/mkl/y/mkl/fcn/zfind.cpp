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

#if 0
        int zfind:: __sign_to_int(const unsigned s)
        {
            switch(s)
            {
                case __z: return 0;
                case __p: return 1;
                case __n: return -1;
                default: break;
            }
            throw libc::exception(EINVAL,"zfind::__sign_to_int");
        }
#endif

        void zfind:: throw_not_bracketed()
        {
             throw libc::exception(EDOM,"zfind: zero is not bracketed");
        }

    }
}

