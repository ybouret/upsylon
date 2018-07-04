
#include "y/concurrent/threads.hpp"
#include "y/type/utils.hpp"
#include "y/code/utils.hpp"
#include "y/os/rt-clock.hpp"

#include <iostream>

namespace upsylon
{
    namespace concurrent
    {
        threads:: ~threads() throw()
        {

            rt_clock clk;
            if(verbose)
            {
                std::cerr << "[threads.quit] halting " << count << " thread" << plural_s(count) << std::endl;
            }

            access.lock();
            dying = true;
            access.unlock();

            synchronize.broadcast();

            while(true)
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
                        access.unlock();
                    }
                }
            }

        }

        threads:: threads(const bool v) :
        __topology( layout::create() ),
        __threads( (*static_cast<__topology *>(this))->cores ),
        access(),
        synchronize(),
        ready(0),
        dying(true),
        verbose(v)
        {
            if(verbose) { std::cerr << "[threads.init] building " << count << " thread" << plural_s(count) << std::endl; }

            // threads init
            try
            {
                for(size_t i=0;i<count;++i)
                {
                    const size_t target = i+1;
                    build<thread_proc,void*>(start,this);
                    while(true)
                    {
                        if( access.try_lock() )
                        {
                            if(ready<target)
                            {
                                access.unlock();
                            }
                            else
                            {
                                access.unlock();
                                break;
                            }
                        }
                    }
                }
            }
            catch(...)
            {
                synchronize.broadcast();
                throw;
            }
            if(verbose) { std::cerr << "[threads.init] are built " << count << " thread" << plural_s(count)  << std::endl; }

            // threads setup
            __topology &self = *this;
            for(size_t i=0;i<count;++i)
            {
                thread &thr = (*this)[i];
                (size_t&)(thr.rank) = i;
                (size_t&)(thr.size) = count;
                const size_t icpu = self->core_index_of(i);
                std::cerr << "thread #" << i << " on cpu #" << icpu << std::endl;
            }


        }


        void threads:: start( void *args ) throw()
        {
            assert(args);
            static_cast<threads *>(args)->loop();
        }

    }
}

namespace upsylon
{
    namespace concurrent
    {
        void threads:: loop() throw()
        {
            //__________________________________________________________________
            //
            // entering thread
            //__________________________________________________________________
            access.lock();
            ++ready; //!< for constructor
            if(verbose) { std::cerr << "[threads.loop] \tready=" << ready << "/" << count << std::endl; }
        LOOP:
            //__________________________________________________________________
            //
            // wait on the LOCKED mutex
            //__________________________________________________________________
            synchronize.wait(access);

            //__________________________________________________________________
            //
            // woke on with the LOCKED mutex
            //__________________________________________________________________
            if(dying)
            {
                if(verbose) { std::cerr << "[threads.loop] \thalting" << std::endl; }
                assert(ready>0);
                --ready;
                access.unlock();
                return;
            }

            // do something...

            goto LOOP;
            
        }
    }

}
