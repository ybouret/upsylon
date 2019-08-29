#include "y/alea.hpp"
#include "y/randomized/marsaglia.hpp"

namespace upsylon
{

    static randomized::Kiss32 __alea;
    randomized::bits  &alea = __alea;
    void alea_init() throw()
    {
        __alea.initialize();
    }
}
