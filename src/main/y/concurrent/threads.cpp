
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

            if(verbose)
            {
                Y_LOCK(access);
                std::cerr << "[threads.quit] halting " << count << " thread" << plural_s(count) << std::endl;
            }

            access.lock();
            dying = true;
            access.unlock();

            flush();
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
        running(0),
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
            if(verbose) { std::cerr << "[threads.init] are built " << count << " thread" << plural_s(count)  << std::endl; }

            // threads setup
            __topology &self = *this;
            if(verbose)
            {
                std::cerr << "\t|_leading  @cpu" << self->core_index_of(0) << std::endl;
            }
            nucleus::thread::assign(nucleus::thread::get_current_handle(),self->core_index_of(0));
            for(size_t i=0;i<count;++i)
            {
                thread &thr = (*this)[i];
                (size_t&)(thr.rank) = i;
                (size_t&)(thr.size) = count;
                const size_t icpu = self->core_index_of(i);
                if(verbose)
                {
                    std::cerr << "\t|_thread #" << i << "@cpu" << icpu << std::endl;
                }
                nucleus::thread::assign(thr.handle,icpu);
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

        void threads:: run(parallel &ctx)
        {
            Y_LOCK(access);
            std::cerr << "\trun context " << ctx.size << "." << ctx.rank << std::endl;
        }


        void threads:: flush() throw()
        {
            if(verbose)
            {
                Y_LOCK(access);
                std::cerr << "[threads] flusing" << std::endl;
            }

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
            ++ready; //!< for constructor
            //const nucleus::thread::ID id = nucleus::thread::get_current_id();
            if(verbose) { std::cerr << "[threads.loop] \tready=" << ready << "/" << count << std::endl; }
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
                if(verbose) { std::cerr << "[threads.loop] \thalting" << std::endl; }
                assert(ready>0);
                --ready;
                access.unlock();
                return;
            }

            // do something...
            try
            {
                ++((size_t&)running);
                access.unlock();
                run(context);
            }
            catch(...)
            {
                // todo, something went wrong
            }

            access.lock();
            --((size_t&)running);
            goto LOOP;
            
        }
    }

}
