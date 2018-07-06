#include "y/concurrent/scheme/simd.hpp"

namespace upsylon
{
    namespace concurrent
    {

        simd:: simd(const bool v) : threads(v), quit(false)
        {
            // at this points, threads are creating and
            // waiting on the synchronize conditionx
        }

        simd:: ~simd() throw()
        {
            Y_LOCK(access);
            quit = true;
        }
    }
}
