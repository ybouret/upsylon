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
        cycle(),
        threshold( workers.count + 1),
        countdown( threshold ),
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
            assert(threshold==countdown);
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
            synchronized.broadcast();
            Y_MUTEX_PROBE(access,ready<=0);
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
        LOOP:
            //__________________________________________________________________
            // wait on a LOCKED mutex
            synchronized.wait(access);
            --ready;

            //__________________________________________________________________
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

            //__________________________________________________________________
            // UNLOCKED kernel run
            access.unlock();
            kproc(kdata,context,access);

            //__________________________________________________________________
            // cycle barrier
            access.lock();
            assert(countdown>0);
            if(verbose) { std::cerr << "[threads.simd.barrier] countdown=" << countdown << "/" << threshold << std::endl; };
            if(--countdown>0)
            {
                // not finished: wait on a LOCKED mutes
                std::cerr << context.label << " is waiting..." << std::endl;
                cycle.wait(access);

                // wake up on a locked mutex
            }
            else
            {
                // the countdown is reached!
                assert(ready<=0);
                std::cerr << context.label << " has finished" << std::endl;
                if(verbose) { std::cerr << "[threads.simd.barrier] cycled!" << std::endl; }
                countdown = threshold;
                cycle.broadcast();
            }

            // DEBUGGING
            ++ready;
            std::cerr << "ready=" << ready << " from " << context.label << std::endl;
            goto LOOP;
        }

        void simd:: finish() throw()
        {
            access.lock();
            assert(countdown>0);
            if(workers.verbose) { std::cerr << "[threads.simd.barrier] main countdown=" << countdown << "/ " << threshold << std::endl; };
            if(--countdown>0)
            {
                cycle.wait(access);
            }
            else
            {
                // the countdown is reached!
                assert(ready<=0);
                countdown = threshold;
            }
            std::cerr << "-- waiting for sync..." << std::endl;
            access.unlock();
            cycle.broadcast();
            Y_MUTEX_PROBE(access,ready>=workers.size());
        }

    }
}




