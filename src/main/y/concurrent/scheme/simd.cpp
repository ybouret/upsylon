#include "y/concurrent/scheme/simd.hpp"

namespace upsylon
{
    namespace concurrent
    {

        simd:: simd(const bool v) :
        threads(v),
        code(0),
        data(0)
        {
            // at this points, threads are creating and
            // waiting on the synchronize conditionx
        }

        simd:: ~simd() throw()
        {
            
        }

        void simd:: ld( kernel user_code, void *user_data )
        {

            //! assuming flushed
            {
#if !defined(NDEBUG)
                Y_LOCK(access);
                assert(running<=0);
#endif
            }
            code=user_code;
            data=user_data;
            synchronize.broadcast();
        }

        void simd:: run( parallel &context ) throw()
        {
            assert(code);
            code(data,context,access);
        }


    }
}
