#include "y/type/is-aligned.hpp"

namespace upsylon {

    const char * is_aligned:: text( const value v ) throw()
    {
        switch(v)
        {
            case on8:  return "8-bits";
            case on16: return "16-bits";
            case on32: return "32-bits";
            case on64: return "64-bits";
        }
        return "??";
    }

}
