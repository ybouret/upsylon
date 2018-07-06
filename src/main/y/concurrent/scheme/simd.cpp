#include "y/concurrent/scheme/simd.hpp"

namespace upsylon
{
    namespace concurrent
    {

        simd:: simd(const bool v) :
        threads(v),
        proc(0),
        data(0)
        {
            // at this points, threads are creating and
            // waiting on the synchronize conditionx
        }

        simd:: ~simd() throw()
        {
            
        }
    }
}
