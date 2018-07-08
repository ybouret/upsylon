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
        workers(v),
        done(false),
        guard(),
        synchronized(),
        ready(0),
        cycle(),
        threshold( workers.size() + 1),
        countdown( threshold ),
        kproc(0),
        kdata(0)
        {
            workers.run(call,this);
            Y_MUTEX_PROBE(guard,ready>=workers.size());
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
            assert(workers.size()==ready);

            kproc = code;
            kdata = data;
            synchronized.broadcast();
            Y_MUTEX_PROBE(guard,ready<=0);
        }


        void simd:: loop(parallel &context) throw()
        {
            //__________________________________________________________________
            //
            // first initialization
            //__________________________________________________________________
            const bool   verbose = workers.verbose;
            const size_t count   = workers.size();
            guard.lock();

            if(verbose)
            {
                Y_LOCK(workers.access);
                std::cerr << "[threads.simd.loop] @" << context.label << std::endl;
            }
            assert(ready<count);
            ++ready;
            if(verbose&&ready>=count)
            {
                Y_LOCK(workers.access);
                std::cerr << "[threads.simd.loop] synchronized" << std::endl;
            }
        LOOP:
            assert(threshold==countdown);
            //__________________________________________________________________
            //
            // wait on the LOCKED mutex...
            //__________________________________________________________________
            synchronized.wait(guard);
            --ready;


            //__________________________________________________________________
            //
            // wake up on the LOCKED mutex
            //__________________________________________________________________
            if(done)
            {
                if(verbose)
                {
                    Y_LOCK(workers.access);
                    std::cerr << "[threads.simd.done] @" << context.label << std::endl;
                }
                guard.unlock();
                return;
            }

            //__________________________________________________________________
            //
            // perform UNLOCKED code
            //__________________________________________________________________
            if(verbose)
            {
                Y_LOCK(workers.access);
                std::cerr << "[threads.simd.code] @" << context.label << ", ready=" << ready << std::endl;
            }
            guard.unlock();
            assert(kproc);
            kproc(kdata,context,guard);

            guard.lock();
            assert(countdown>0);
            if(--countdown>0)
            {
                if(verbose)
                {
                    Y_LOCK(workers.access);
                    std::cerr << "[threads.simd.loop] @" << context.label << ", countdown=" << countdown << "/" << threshold << std::endl;
                }
                cycle.wait(guard);
                // comme back on a LOCKED mutex
            }
            else
            {
                // still on a LOCKED muteex
            }
            return;

        }

        void simd:: finish() throw()
        {

        }

    }
}




