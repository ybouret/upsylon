#include "y/concurrent/scheme/simd.hpp"
#include <iostream>

namespace upsylon
{
    namespace concurrent
    {

        simd:: simd() :
        for_each(),
        workers(),
        access( workers.access ),
        done(false),
        ready(0),
        cycle(),
        synch(),
        kproc(0),
        kdata(0),
        ended(0),
        verbose( workers.verbose )
        {
            workers.run(call,this);
            const size_t count = workers.size();
            Y_MUTEX_PROBE(access,ready>=count);
        }

        void simd:: call( void *data, parallel &context, lockable &) throw()
        {
            assert(data);
            static_cast<simd *>(data)->loop(context);
        }


        executor & simd:: engine() throw()
        {
            return workers;
        }
        


        simd:: ~simd() throw()
        {
            //__________________________________________________________________
            //
            // will intercept all codes
            //__________________________________________________________________
            access.lock();
            const size_t count = workers.size();
            assert(ready>=count);
            done = true;
            access.unlock();

            //__________________________________________________________________
            //
            // then say bye to synchonized
            //__________________________________________________________________
            cycle.broadcast();
            Y_MUTEX_PROBE(access,ended>=count);

        }

        void simd:: loop(parallel &context) throw()
        {
            //__________________________________________________________________
            //
            //
            // first initialization
            //
            //__________________________________________________________________
            const size_t count   = workers.size();

            access.lock();
            assert(ready<count);
            if(verbose) { std::cerr << "[threads.simd.loop] enter " << context.label << std::endl; }
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
        LOOP:
            //__________________________________________________________________
            //
            // wait on a LOCKED mutex
            //__________________________________________________________________
            cycle.wait(access);

            //__________________________________________________________________
            //
            // wake up on a LOCKED mutex
            //__________________________________________________________________
            if(done)
            {
                if(verbose) { std::cerr << "[threads.simd.loop] leave " << context.label << std::endl; }
                ++ended;
                access.unlock();
                return;
            }

            assert(kproc);
            assert(ready<count);
            if(verbose) { std::cerr << "[threads.simd.loop.code] @" << context.label << std::endl; }

            //__________________________________________________________________
            //
            // UNLOCKED kernel run
            //__________________________________________________________________
            access.unlock();
            kproc(kdata,context,access);

            //__________________________________________________________________
            //
            // cycle barrier
            //__________________________________________________________________
            access.lock();
            if(++ready>=count)
            {
                synch.broadcast();
            }

            goto LOOP;
        }

        void simd:: run(kernel code, void *data)
        {
            access.lock();
            assert(code);
            assert(workers.size()==ready);
            kproc = code;
            kdata = data;
            ready = 0;
            cycle.broadcast();
            synch.wait(access);
            assert(ready>=workers.size());
            access.unlock();
        }

    }
}




