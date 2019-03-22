
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
                if(verbose) { std::cerr << "[threads.quit] " << ready << "/" << engines.count << std::endl; }
            }
            
            start.broadcast();
            Y_MUTEX_PROBE(access,ready<=0);

        }

        size_t     threads:: num_threads() const throw() { return engines.size(); }
        parallel & threads:: get_context(const size_t context_index) throw()
        {
            return engines[context_index];
        }

        threads:: threads(const bool v) :
        __topology( layout::create() ),
        access(),
        engines( (*static_cast<__topology *>(this))->cores ),
        halting(true),
        ready(0),
        start(),
        kproc(0),
        kdata(0),
        verbose(v)
        {
            //__________________________________________________________________
            //
            // threads init
            //__________________________________________________________________
            const size_t count = engines.count;
            if(verbose) { std::cerr << "[threads.init] build " << count << " thread" << plural_s(count) << "..." << std::endl; }

            try
            {
                // construct with halting=true;
                for(size_t i=0;i<count;++i)
                {
                    const size_t target = i+1; //! desired new number of threads
                    engines.build<thread_proc,void*,size_t,size_t>(system_entry,this,count,i);
                    Y_MUTEX_PROBE(access,ready>=target);
                }

            }
            catch(...)
            {
                start.broadcast();
                Y_MUTEX_PROBE(access,ready<=0);
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
            __threads &thr = engines;
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


        void threads:: system_entry( void *args ) throw()
        {
            assert(args);
            static_cast<threads *>(args)->thread_entry();
        }

        void threads:: thread_entry() throw()
        {
            //__________________________________________________________________
            //
            // entering thread
            //__________________________________________________________________
            access.lock();
            parallel &context = engines[ready];
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

            assert(kproc);
            kproc(kdata,context,access);

            
            if(verbose)
            {
                Y_LOCK(access);
                std::cerr << "[threads.loop] back@" << context.label << std::endl;
            }
            access.lock();
            --ready;
            if(verbose) { std::cerr << "[threads.quit.done] (-) " << context.label << std::endl; }
            access.unlock();
            return;

        }



        void threads:: run(kernel code, void *data)
        {
            static const char fn[] = "[threads.run!]";
            assert(code);
            {
                Y_LOCK(access);
                if(verbose) { std::cerr << fn << std::endl; }
                if(ready<engines.size()) { throw exception("%s unexpected unfinished setup",fn); }
                if(!halting)             { throw exception("%s already setup run",fn);           }
                halting = false;
                kproc   = code;
                kdata   = data;
            }
            start.broadcast();
        }

    }
}


