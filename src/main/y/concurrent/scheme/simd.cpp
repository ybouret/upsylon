#include "y/concurrent/scheme/simd.hpp"
#include <iostream>

namespace upsylon
{
    namespace concurrent
    {
        simd:: ~simd() throw()
        {
        }

        simd:: simd(const bool v) :
        for_each(),
        workers(v)
        {
            workers.run(call,this);
        }

        void simd:: call( void *data, parallel &context, lockable &access) throw()
        {
            assert(data);
            {
                Y_LOCK(access);
                std::cerr << "\tcall simd@" << context.label << std::endl;
            }
            static_cast<simd *>(data)->loop(context,access);
        }


        executor & simd:: engine() throw()
        {
            return workers;
        }

        void simd:: finish() throw()
        {
        }

        void simd:: start(kernel code, void *data)
        {
            assert(code);
        }


        void simd:: loop(parallel &context, lockable &access) throw()
        {
            {
                Y_LOCK(access);
                std::cerr << "\tloop simd@" << context.label << std::endl;
            }
        }

    }
}


#if 0
namespace upsylon
{
    namespace concurrent
    {

        simd:: simd(const bool v) :
        threads(v),
        threshold( size()+1 ),
        counter(0),
        cycle(),
        code(0),
        data(0)
        {
            // at this points, threads are creating and
            // waiting on the synchronize conditionx
        }

        simd:: ~simd() throw()
        {
            
        }

        void simd:: start( kernel user_code, void *user_data )
        {

            //__________________________________________________________________
            //
            // assuming at a good starting point
            //__________________________________________________________________
            {
#if !defined(NDEBUG)
                Y_LOCK(access);
                assert(running<=0);
#endif
            }
            code    = user_code; //!< will be call multiple times
            data    = user_data; //!< on those data
            counter = threshold; //!< for cycle
            synchronize.broadcast();

        }

        void simd:: run( parallel &context ) throw()
        {
            assert(code);
            code(data,context,access);

            //__________________________________________________________________
            //
            // internal thread barrier
            //__________________________________________________________________
            access.lock();
            assert(counter>0);
            //std::cerr << "counter=" << counter << "@thread " << context.label << std::endl;
            if(--counter>0)
            {
                cycle.wait(access);
            }
            else
            {
                cycle.broadcast();
            }
            access.unlock();
        }

        void simd:: finish() throw()
        {
            //__________________________________________________________________
            //
            // internal thread barrier
            //__________________________________________________________________
            access.lock();
            //std::cerr << "counter=" << counter << "@main" << std::endl;
            if(--counter>0)
            {
                cycle.wait(access);
            }
            else
            {
                cycle.broadcast();
            }
            access.unlock();

            //__________________________________________________________________
            //
            // let threads go back to synchronize
            //__________________________________________________________________
            wait();
        }


    }
}
#endif

