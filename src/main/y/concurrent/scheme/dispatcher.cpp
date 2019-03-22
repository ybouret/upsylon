
#include "y/concurrent/scheme/dispatcher.hpp"

namespace upsylon
{
    namespace concurrent
    {

        dispatcher:: jnode:: ~jnode() throw()
        {
            (int&)valid = 0;
        }

        dispatcher:: jnode:: jnode( const job_uuid u, const job_type &J ) :
        next(0), prev(0), uuid(u), call(J), valid(1)
        {
        }

    }

}

namespace upsylon
{
    namespace concurrent
    {
        dispatcher:: jpool:: jpool() throw()  : jpool_type() {}

        dispatcher:: jpool::  ~jpool() throw()
        {
            clear();
        }

        dispatcher::jnode * dispatcher:: jpool:: fetch()
        {
            if(size)
            {
                return checked( query() );
            }
            else
            {
                return object::acquire1<jnode>();
            }
        }

        void dispatcher:: jpool:: clear() throw()
        {
            while(size)
            {
                jnode *j = checked( query() );
                object::release1(j);
            }
        }

        void dispatcher:: jpool:: gc() throw()
        {
            for( jnode *j = top; j; j=checked(j)->next)
                ;
        }


        dispatcher::jnode * dispatcher:: jpool:: checked( jnode *j ) throw()
        {
            assert(j);
            if(j->valid) { destruct(j); assert(0==j->valid); }
            return j;
        }

    }

}

namespace upsylon
{
    namespace concurrent
    {



        dispatcher:: dispatcher(const bool v) :
        pending(),
        current(),
        aborted(),
        storage(),
        workers(v),
        access( workers.access ),
        prepared(0),
        stopping(false),
        activity(),
        flushing(),
        verbose( workers.verbose )
        {
            // and start
            workers.run(start,this);

            // wait for initial synch
            Y_MUTEX_PROBE(access,prepared>=workers.count);

            if(verbose)
            {
                std::cerr << "|_[dispatcher: ready]" << std::endl;
            }
        }

        executor & dispatcher:: engine() throw()
        {
            return workers;
        }


        void dispatcher:: remove_pending() throw()
        {
            Y_LOCK(access);
            while( pending.size )
            {
                jnode *j = pending.pop_back();
                destruct(j);
                storage.store(j);
            }
        }

        void dispatcher:: reserve_jobs( size_t n )
        {
            Y_LOCK(access);
            while(n-->0)
            {
                storage.store( object::acquire1<jnode>() );
            }
        }


      

        job_uuid dispatcher:: enqueue( const job_type &job )
        {
            Y_LOCK(access);
            jnode *j = storage.fetch();
            try
            {
                new (j) jnode(uuid,job);
                ++uuid;
                pending.push_back(j);
                activity.signal();
                return j->uuid;
            }
            catch(...)
            {
                (int&)(j->valid)=0; // mark as not constructed in any case
                storage.store(j);
                throw;
            }
        }


        void dispatcher:: flush() throw()
        {
            access.lock();
            if(verbose)
            {
                std::cerr << "|_[dispatcher.flush request with stopping=" << (stopping?"true":"false") << "]" << std::endl;
            }

            if(pending.size>0||current.size>0)
            {
                // wait on a locked muted
                flushing.wait(access);

                // wake up on a locked mutex
                std::cerr << "|_[dispatcher.flush achived with stopping=" << (stopping?"true":"false") << "]" << std::endl;
            }
            access.unlock();
        }

        void dispatcher:: start(void     *addr,
                                parallel &context,
                                lockable &)
        {
            assert(addr);
            dispatcher &self = *static_cast<dispatcher *>(addr);
            self.loop(context);
        }


        dispatcher:: ~dispatcher() throw()
        {
            // take control
            access.lock();

            // next activity will be to comeback
            stopping=true;

            if(verbose)
            {
                std::cerr << "|_[dispatcher.quit: remaining #jobs=" << pending.size  << "]" << std::endl;
            }

            //  suppression of pending jobs
            while( pending.size )
            {
                jnode *j = pending.pop_back();
                destruct(j);
                object::release1(j);
            }

            // flush current threads
            access.unlock();
            flush();

            // prepare for final call
            access.lock();
            activity.broadcast();
            access.unlock();

            // wait for final synch
            Y_MUTEX_PROBE(access,prepared<=0);

            assert(0==pending.size);
            assert(0==current.size);
            // and memory is releaded by storage and aborted


        }

        void dispatcher:: loop(parallel &context)
        {
            //==================================================================
            //
            // initialisation
            //
            //==================================================================

            access.lock();
            if(verbose)
            {
                std::cerr << "|_[dispatcher.loop@" << context.size << "." << context.rank << "]" << std::endl;
            }
            assert(prepared<workers.count);
            ++prepared;
            if(verbose&&prepared>=workers.count)
            {
                std::cerr << "|_[dispatcher.synchronized]" << std::endl;
            }


            //==================================================================
            //
            // loop to wait for being woken up
            //
            //==================================================================
        CYCLE:
            // wait on a LOCKED muted
            activity.wait(access);

            // wake up on a locked mutes
            if(stopping)
            {
                if(verbose) { std::cerr << "|_[dispatcher.done@"<< context.size << "." << context.rank << "]" << std::endl; }
                assert(prepared>0);
                --prepared;
                access.unlock();
                return;
            }



        }

    }
}
