#include "y/concurrent/scheme/simd.hpp"
#include <iostream>

namespace upsylon
{
    namespace concurrent
    {

        simd:: simd(const bool v) :
        for_each(),
        workers(v),
        access( workers.access ),
        done(false),
        synchronized(),
        ready(0),
        kproc(0),
        kdata(0)
        {
            workers.run(call,this);
            Y_MUTEX_PROBE(access,ready>=workers.count);
        }

        void simd:: call( void *data, parallel &context, lockable &sync) throw()
        {
            assert(data);
            {
                Y_LOCK(sync);
                std::cerr << "[threads.simd.call] @" << context.label << std::endl;
            }
            static_cast<simd *>(data)->loop(context);
        }


        executor & simd:: engine() throw()
        {
            return workers;
        }



        void simd:: start(kernel code, void *data)
        {
            assert(code);
            assert(workers.count==ready);

            kproc = code;
            kdata = data;
            synchronized.broadcast();


        }

        simd:: ~simd() throw()
        {
            // will intercept all codes
            {
                Y_LOCK(access);
                done = true;
            }

            // wait for current kernels to finish

            // then say bye to synchonized
            // no choice but to signal until ready<=0
            synchronized.broadcast();
            Y_MUTEX_PROBE(access,ready<=0);

            while(false)
            {
                if(access.try_lock())
                {
                    if(ready<=0)
                    {
                        access.unlock();
                        break;
                    }
                    else
                    {
                        synchronized.signal();
                        access.unlock();
                    }
                }
            }
            
        }

        void simd:: loop(parallel &context) throw()
        {
            //__________________________________________________________________
            //
            //
            // first initialization
            //
            //__________________________________________________________________
            const bool   verbose = workers.verbose;
            const size_t count   = workers.count;

            access.lock();
            assert(ready<workers.count);
            if(verbose)
            {
                std::cerr << "[threads.simd.loop] enter " << context.label << std::endl;
            }
            ++ready;
            if(verbose&&ready>=count)
            {
                std::cerr << "[threads.simd.loop] synchronized" << std::endl;
            }

            //__________________________________________________________________
            //
            //
            // LOOP:
            //
            //__________________________________________________________________

            // wait on a LOCKED mutex
            synchronized.wait(access);
            --ready;

            // waking up on a LOCKED mutex
            if(done)
            {
                if(verbose)
                {
                    std::cerr << "[threads.simd.loop] leave " << context.label << std::endl;
                }
                access.unlock();
                return;
            }

            assert(kproc);
            if(verbose)
            {
                std::cerr << "[threads.simd.code] @" << context.label << std::endl;
            }
            access.unlock();
            kproc(kdata,context,access);
            
            return;

        }

        void simd:: finish() throw()
        {

        }

    }
}




