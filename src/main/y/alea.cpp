#include "y/alea.hpp"
#include "y/random/marsaglia.hpp"

namespace upsylon
{

    static random::Kiss32 __alea;
    random::bits &alea = __alea;
    void alea_init() throw()
    {
        __alea.initialize();
    }
}
