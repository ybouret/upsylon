
#include "y/concurrent/threads.hpp"
#include "y/type/utils.hpp"
#include "y/code/utils.hpp"
#include "y/exceptions.hpp"

#include <iostream>

namespace upsylon
{
    namespace concurrent
    {
        threads:: ~threads() throw()
        {


            {
                Y_LOCK(access);
                if(verbose) { std::cerr << "[threads.quit] " << ready << "/" << count << std::endl; }
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

            //__________________________________________________________________
            //
            // unlock call
            //__________________________________________________________________
            if(verbose)
            {
                Y_LOCK(access);
                std::cerr << "[threads.loop] call@" << context.label << std::endl;
            }

            
            if(verbose)
            {
                Y_LOCK(access);
                std::cerr << "[threads.loop] back@" << context.label << std::endl;
            }
            access.lock();
            if(verbose)
            --ready;
            if(verbose) { std::cerr << "[threads.quit.done] (-) " << context.label << std::endl; }
            access.unlock();
            return;

        }



        void threads:: run()
        {
            static const char fn[] = "[threads.run!]";
            Y_LOCK(access);
            if(verbose) { std::cerr << fn << std::endl; }
            if(ready<size()) throw exception("%s unexpected unfinished setup",fn);
            if(!halting) throw exception("%s already setup run",fn);
            halting = false;
            start.broadcast();
        }

    }
}


