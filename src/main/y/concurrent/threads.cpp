
#include "y/concurrent/threads.hpp"
#include "y/type/utils.hpp"
#include "y/code/utils.hpp"

#include <iostream>

namespace upsylon
{
    namespace concurrent
    {
        threads:: ~threads() throw()
        {

            //__________________________________________________________________
            //
            // set status to dying
            //__________________________________________________________________
            {
                Y_LOCK(access);
                dying = true;
                if(verbose)
                {
                    std::cerr << "[threads.quit] halting " << count << " thread" << plural_s(count) << std::endl;
                }
            }
            //__________________________________________________________________
            //
            // wait for running threads to complete
            //__________________________________________________________________
            wait();

            //__________________________________________________________________
            //
            // then wake up everyone on dying status
            //__________________________________________________________________
            synchronize.broadcast();

            //__________________________________________________________________
            //
            // and probe for all done...
            //__________________________________________________________________
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
        running(0),
        access(),
        synchronize(),
        ready(0),
        dying(true),
        verbose(v)
        {
            //__________________________________________________________________
            //
            // threads init
            //__________________________________________________________________
            if(verbose) { std::cerr << "[threads.init] build " << count << " thread" << plural_s(count) << "..." << std::endl; }

            try
            {
                for(size_t i=0;i<count;++i)
                {
                    const size_t target = i+1;
                    build<thread_proc,void*,size_t,size_t>(start,this,count,i);
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
                                dying = false;   //!< ready for processing
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
            if(verbose) { std::cerr << "[threads.init] built " << count << " thread" << plural_s(count)  << "!!!" << std::endl; }

            //__________________________________________________________________
            //
            // threads setup
            //__________________________________________________________________
            // place leading threads
            __topology &topo = *this;
            if(verbose)
            {
                std::cerr << "[threads.init.affinity] main@cpu" << topo->core_index_of(0) << std::endl;
            }
            nucleus::thread::assign(nucleus::thread::get_current_handle(),topo->core_index_of(0));

            // place other threads
            __threads &thr = *this;
            for(size_t i=0;i<count;++i)
            {
                const size_t icpu = topo->core_index_of(i);
                if(verbose)
                {
                    std::cerr << "[threads.init.affinity]   #" << i << "@cpu" << icpu << std::endl;
                }
                nucleus::thread::assign(thr[i].handle,icpu);
            }
            if(verbose) { std::cerr << "[threads.init] ready to work..." << std::endl; }

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

        void threads:: run(parallel &ctx) throw()
        {
            Y_LOCK(access);
            std::cerr << "\trun context " << ctx.label << std::endl;
        }


        void threads:: wait() throw()
        {

#if 0
            if(verbose)
            {
                Y_LOCK(access);
                std::cerr << "[threads.wait] ..." << std::endl;
            }
#endif
            while( true )
            {
                if( access.try_lock() )
                {
                    if(running>0)
                    {
                        access.unlock();
                        continue;
                    }
                    else
                    {
                        access.unlock();
                        return;
                    }
                }
            }
        }

        void threads:: loop() throw()
        {
            //__________________________________________________________________
            //
            // entering thread
            //__________________________________________________________________
            access.lock();
            parallel &context = static_cast<__threads&>(*this)[ready];
            if(verbose) { std::cerr << "[threads.init.call] (+) " << context.label << std::endl; }
            ++ready; //!< for constructor
        LOOP:
            //__________________________________________________________________
            //
            // wait on the LOCKED mutex
            //__________________________________________________________________
            synchronize.wait(access);

            //__________________________________________________________________
            //
            // wake up on with the LOCKED mutex
            //__________________________________________________________________
            if(dying)
            {
                if(verbose) { std::cerr << "[threads.loop.halt] (-) " << context.label << std::endl; }
                assert(ready>0);
                --ready;
                access.unlock();
                return;
            }

            //__________________________________________________________________
            //
            // do something, still starting with a LOCKED mutex
            //__________________________________________________________________
            try
            {
                ++((size_t&)running);
                if(verbose) { std::cerr << "(+)running: " << running << "/" << context.size << std::endl; }
                access.unlock();
                run(context); // virtual call
            }
            catch(...)
            {
                // todo, something went wrong
            }

            
            access.lock();
            --((size_t&)running);
            if(verbose) { std::cerr << "(-)running: " << running << "/" << context.size << std::endl; }
            goto LOOP;
            
        }
    }

}
