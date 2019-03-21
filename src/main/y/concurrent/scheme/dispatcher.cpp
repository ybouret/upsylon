
#include "y/concurrent/scheme/dispatcher.hpp"

namespace upsylon
{
    namespace concurrent
    {

        dispatcher:: jnode:: ~jnode() throw()
        {
        }

        dispatcher:: jnode:: jnode( const job_uuid u, const job_type &J ) :
        next(0), prev(0), uuid(u), call(J)
        {
        }

    }

}

namespace upsylon
{
    namespace concurrent
    {
        dispatcher:: ~dispatcher() throw()
        {
            {
                Y_LOCK(workers.access);
                done=true;

                if(verbose) {
                    std::cerr << "** [dispatcher.quit: remaining #jobs=" << jobs.size  << "]" << std::endl;
                    std::cerr << "** [dispatcher.quit: ready=" << ready << "/" << workers.count << "]" << std::endl;
                }

                // direct suppression of pending jobs
                while( jobs.size )
                {
                    jnode *j = jobs.pop_back();
                    destruct(j);
                    object::release1(j);
                }

                // wait for current jobs to finish
                if(ready<workers.count)
                {
                    // wait on a locked mutex
                    synch.wait(workers.access);
                    // come back on a locked mutex
                }

                cycle.broadcast();
            }
            
            assert(0==jobs.size);

            // and final clear
            jmem.clear();

        }


        dispatcher:: dispatcher(const bool v) :
        jobs(),
        jmem(),
        workers(v),
        done(false),
        ready(0),
        cycle(),
        verbose( workers.verbose )
        {
            // and start
            workers.run(start,this);

            // wait for initial synch
            Y_MUTEX_PROBE(workers.access,ready>=workers.count);

            if(verbose)
            {
                std::cerr << "** [dispatcher: ready]" << std::endl;
            }
        }

        executor & dispatcher:: engine() throw()
        {
            return workers;
        }


        void dispatcher:: remove_pending() throw()
        {
            Y_LOCK(workers.access);
            while( jobs.size )
            {
                jnode *j = jobs.pop_back();
                destruct(j);
                jmem.store(j);
            }
        }

        void dispatcher:: reserve_jobs( size_t n )
        {
            Y_LOCK(workers.access);
            while(n-->0)
            {
                jmem.store( object::acquire1<jnode>() );
            }
        }


        bool    dispatcher:: is_done( const job_uuid  jid ) const throw()
        {
            Y_LOCK(workers.access);
            for(const jnode *j=jobs.head;j;j=j->next)
            {
                if(j->uuid==jid) return false;
            }
            return true;
        }

        bool dispatcher:: is_live(const job_uuid jid) const throw()
        {
            Y_LOCK(workers.access);
            for(const jnode *j=jobs.head;j;j=j->next)
            {
                if(j->uuid==jid) return true;
            }
            return false;
        }

        job_uuid dispatcher:: enqueue( const job_type &job )
        {
            Y_LOCK(workers.access);
            jnode *j = jmem.fetch();
            try
            {
                new (j) jnode(uuid,job);
                ++uuid;
                return j->uuid;
            }
            catch(...)
            {
                jmem.store(j);
                throw;
            }
        }


        void dispatcher:: join() throw()
        {
            if(verbose)
            {
                std::cerr << "** [dispatcher.join done=" << (done?"true":"false") << "]" << std::endl;
            }
        }

        void dispatcher:: start(void     *addr,
                                parallel &context,
                                lockable &)
        {
            assert(addr);
            dispatcher &self = *static_cast<dispatcher *>(addr);
            self.loop(context);
        }

        void dispatcher:: loop(parallel &context)
        {
            mutex &access = workers.access;


            access.lock();
            if(verbose)
            {
                std::cerr << "** [dispatcher.loop@" << context.size << "." << context.rank << "]" << std::endl;
            }
            assert(ready<workers.count);
            ++ready;
            if(verbose&&ready>=workers.count)
            {
                std::cerr << "** [dispatcher.synchronized]" << std::endl;
            }

        LOOP:
            // wait on the lock mutex
            cycle.wait(access);

            if(verbose) { std::cerr << " dispatch loop is called!" << std::endl; }

            // wake up on the LOCKED mutex
            if(done)
            {
                if(verbose) { std::cerr << " dispatch loop is done" << std::endl; }
                access.unlock();
                return;
            }

            // mark this thread as engaged in a computation
            --ready;

            // blah, blah

            // end of loop
            access.lock();
            if(++ready)
            {
                synch.broadcast();
            }
            goto LOOP;

        }

    }
}
