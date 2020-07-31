
#include "y/concurrent/threads.hpp"
#include "y/type/utils.hpp"
#include "y/code/utils.hpp"
#include "y/exception.hpp"

#include <iostream>

namespace upsylon
{
    namespace concurrent
    {

#define Y_THREAD_PRINTLN(OUTPUT) do { if(verbose) { std::cerr << OUTPUT << std::endl;}  } while(false)

        threads:: ~threads() throw()
        {
            {
                Y_LOCK(access);
                Y_THREAD_PRINTLN("[threads.quit] " << ready << "/" << engines.count);
            }
            start.broadcast();
            Y_MUTEX_PROBE(access,ready<=0);
        }

        size_t     threads:: num_threads() const throw() { return engines.size(); }

        parallel & threads:: get_context(const size_t context_index) throw()
        {
            return engines[context_index];
        }

#define Y_THREADS_CTOR(LAYOUT) \
topology( LAYOUT ),            \
access(),                      \
engines(topology->cores),      \
halting(true),                 \
ready(0),                      \
start(),                       \
kproc(0),                      \
kdata(0),                      \
verbose(get_verbosity())

        threads:: threads() :
        Y_THREADS_CTOR( layout::create() ) 
        {
            initialize();
        }

        void threads:: initialize()
        {
            //__________________________________________________________________
            //
            // threads init
            //__________________________________________________________________
            const size_t count = engines.count;
            Y_THREAD_PRINTLN("[threads.init] build " << count << " thread" << plural_s(count) << "...");
            try
            {
                // construct with halting=true;
                for(size_t i=0,target=1;i<count;++i,++target)
                {
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
            Y_THREAD_PRINTLN("[threads.init] built " << count << " thread" << plural_s(count));

            //__________________________________________________________________
            //
            // threads setup
            //__________________________________________________________________

            // place leading thread
            Y_THREAD_PRINTLN("[threads.init.affinity] main@cpu" << topology->core_index_of(0));
            nucleus::thread::assign(nucleus::thread::get_current_handle(),topology->core_index_of(0));

            // place other threads
            __threads &thr = engines;
            for(size_t i=0;i<count;++i)
            {
                const size_t icpu = topology->core_index_of(i);
                Y_THREAD_PRINTLN("[threads.init.affinity]   #" << i << "@cpu" << icpu);
                nucleus::thread::assign(thr[i].handle,icpu);
            }
            Y_THREAD_PRINTLN("[threads.init] ready to work...");
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
            ++ready; //!< for constructor
            Y_THREAD_PRINTLN("[threads.init.call] (+) " << context.label << " -> " << ready);

            //__________________________________________________________________
            //
            // ready to work!
            //__________________________________________________________________
            start.wait(access); //!< wait on locked mutex, return unlocked


            //__________________________________________________________________
            //
            // first wake up on the LOCKED access
            //__________________________________________________________________
            if(halting)
            {
                Y_THREAD_PRINTLN("[threads.quit.nope] (-) " << context.label);
                --ready;
                access.unlock();
                return;
            }
            Y_THREAD_PRINTLN("[threads.loop] call@" << context.label);
            access.unlock();

            //__________________________________________________________________
            //
            // unlocked call
            //__________________________________________________________________

            assert(kproc);
            kproc(kdata,context,access);


            access.lock();
            //Y_THREAD_PRINTLN("[threads.loop] back@" << context.label);
            --ready;
            Y_THREAD_PRINTLN("[threads.quit.done] (-) " << context.label << " -> " << ready);
            access.unlock();
            return;

        }



        void threads:: run(kernel code, void *data)
        {
            static const char fn[] = "[threads.run]";
            assert(code);
            {
                Y_LOCK(access);
                Y_THREAD_PRINTLN(fn);
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


