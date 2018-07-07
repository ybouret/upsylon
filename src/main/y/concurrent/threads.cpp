
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
            // in any case, place the halting flag to true
            //__________________________________________________________________
            {
                Y_LOCK(access);
                halting = true;
                if(verbose) { std::cerr << "[threads.quit]" << std::endl; }
            }

            // no choice but to signal until ready<=0
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
                        start.signal();
                        access.unlock();
                    }
                }
            }


        }

        threads:: threads(const bool v) :
        __topology( layout::create() ),
        __threads( (*static_cast<__topology *>(this))->cores ),
        access(),
        halting(true),
        ready(0),
        start(),
        verbose(v)
        {
            //__________________________________________________________________
            //
            // threads init
            //__________________________________________________________________
            if(verbose) { std::cerr << "[threads.init] build " << count << " thread" << plural_s(count) << "..." << std::endl; }

            try
            {
                // construct with halting=true;
                for(size_t i=0;i<count;++i)
                {
                    const size_t target = i+1; //! desired new number of threads
                    build<thread_proc,void*,size_t,size_t>(entry,this,count,i);
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

                access.lock();
                access.unlock();


            }
            catch(...)
            {
                start.broadcast();
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


        void threads:: entry( void *args ) throw()
        {
            assert(args);
            static_cast<threads *>(args)->start_thread();
        }

        void threads:: start_thread() throw()
        {
            //__________________________________________________________________
            //
            // entering thread
            //__________________________________________________________________
            access.lock();
            parallel &context = static_cast<__threads&>(*this)[ready];
            if(verbose) { std::cerr << "[threads.init.call] (+) " << context.label << std::endl; }
            ++ready; //!< for constructor
            //__________________________________________________________________
            //
            // ready to work!
            //__________________________________________________________________
            start.wait(access);


            //__________________________________________________________________
            //
            // first wake up on the LOCKED access
            //__________________________________________________________________
            if(halting)
            {
                if(verbose) { std::cerr << "[threads.quit.nope] (-) " << context.label << std::endl; }
                --ready;
                access.unlock();
                return;
            }
            access.unlock();



            std::cerr << "do stuff...@" << context.label << std::endl;

            access.lock();
            if(verbose)
            --ready;
            if(verbose) { std::cerr << "[threads.quit.done] (-) " << context.label << std::endl; }
            access.unlock();
            return;

        }

    }
}

namespace upsylon
{
    namespace concurrent
    {

#if 0
        void threads:: run(parallel &ctx) throw()
        {
            Y_LOCK(access);
            std::cerr << "\trun context " << ctx.label << std::endl;
        }
#endif


#if 0
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
#endif

#if 0
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
            if(running<=0&&dying)
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
            if(running>=size())
            {
                ( (size_t&) running ) = 0;
                if(dying)
                {
                    access.unlock();
                    return;
                }
            }
            else
            {
                // running < size
                std::cerr << context.label << " waiting on finalize..." << std::endl;
                finalize.wait(access);
            }
            //--((size_t&)running);
            //if(verbose) { std::cerr << "(-)running: " << running << "/" << context.size << std::endl; }
            goto LOOP;
            
        }
#endif
    }

}
