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
        ready(0),
        cycle(),
        kproc(0),
        kdata(0),
        verbose( workers.verbose )
        {
            workers.run(call,this);
            Y_MUTEX_PROBE(access,ready>=workers.count);
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

            // will intercept all codes
            {
                Y_LOCK(access);
                assert(ready>=workers.size());
                done = true;
            }



            // then say bye to synchonized
            cycle.broadcast();
        }

        void simd:: loop(parallel &context) throw()
        {
            //__________________________________________________________________
            //
            //
            // first initialization
            //
            //__________________________________________________________________
            const size_t count   = workers.count;

            access.lock();
            assert(ready<workers.count);
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
                access.unlock();
                return;
            }

            assert(kproc);
            assert(ready<=0); //! ready was set by run() to zero
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

            //std::cerr << "ready=" << ready << " from " << context.label << std::endl;
            goto LOOP;
        }

        void simd:: run(kernel code, void *data)
        {
            access.lock();
            assert(code);
            assert(workers.count==ready);
            kproc = code;
            kdata = data;
            ready = 0;
            cycle.broadcast();
            synch.wait(access);
            assert(ready>=workers.count);
            access.unlock();

        }

    }
}




