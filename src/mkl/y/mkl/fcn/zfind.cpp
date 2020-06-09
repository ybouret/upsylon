#include "y/math/fcn/zfind.hpp"
#include "y/exception.hpp"


namespace upsylon
{
    namespace math
    {
        const unsigned zfind::__z;
        const unsigned zfind::__n;
        const unsigned zfind::__p;

        const char *zfind:: sign_text(const unsigned) throw()
        {
            return "unknown";
        }

        void zfind:: throw_not_bracketed()
        {
             throw exception("zfind: zero is not bracketed");
        }
    }
}

