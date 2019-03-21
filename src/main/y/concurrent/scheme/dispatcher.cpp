
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
        jobs(),
        junk(),
        jerr(),
        workers(v),
        access( workers.access ),
        done(false),
        ready(0),
        cycle(),
        verbose( workers.verbose )
        {
            // and start
            workers.run(start,this);

            // wait for initial synch
            Y_MUTEX_PROBE(access,ready>=workers.count);

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
            Y_LOCK(access);
            while( jobs.size )
            {
                jnode *j = jobs.pop_back();
                destruct(j);
                junk.store(j);
            }
        }

        void dispatcher:: reserve_jobs( size_t n )
        {
            Y_LOCK(access);
            while(n-->0)
            {
                junk.store( object::acquire1<jnode>() );
            }
        }


        bool    dispatcher:: is_done( const job_uuid  jid ) const throw()
        {
            Y_LOCK(access);
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
            Y_LOCK(access);
            jnode *j = junk.fetch();
            try
            {
                new (j) jnode(uuid,job);
                ++uuid;
                jobs.push_back(j);
                cycle.signal();
                return j->uuid;
            }
            catch(...)
            {
                (int&)(j->valid)=0; // mark as not constructed in any case
                junk.store(j);
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


        dispatcher:: ~dispatcher() throw()
        {
            access.lock();
            done=true;

            if(verbose)
            {
                std::cerr << "** [dispatcher.quit: remaining #jobs=" << jobs.size  << "]" << std::endl;
                std::cerr << "** [dispatcher.quit: ready="           << ready      << "/" << workers.count << "]" << std::endl;
            }

            // direct suppression of pending jobs
            while( jobs.size )
            {
                jnode *j = jobs.pop_back();
                destruct(j);
                object::release1(j);
            }

            cycle.broadcast();

            // and access is unlocked
            access.unlock();
            Y_MUTEX_PROBE(access,ready>=workers.count);

            assert(0==jobs.size);


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
                std::cerr << "** [dispatcher.loop@" << context.size << "." << context.rank << "]" << std::endl;
            }
            assert(ready<workers.count);
            ++ready;
            if(verbose&&ready>=workers.count)
            {
                std::cerr << "** [dispatcher.synchronized]" << std::endl;
            }


            //==================================================================
            //
            // loop to wait for being woken up
            //
            //==================================================================
        CYCLE:
            //------------------------------------------------------------------
            //
            // wait on the LOCKED mutex
            //
            //------------------------------------------------------------------
            cycle.wait(access);

            //------------------------------------------------------------------
            //
            // wake up on the LOCKED mutex
            //
            //------------------------------------------------------------------
            if(verbose) { std::cerr << "** [dispatcher.call@" << context.size << "." << context.rank << "]" << std::endl; }
            if(verbose) { std::cerr << "** [dispatcher.call@" << context.size << "." << context.rank << "] jobs=" << jobs.size << std::endl; }

            if(done)
            {
                if(verbose) { std::cerr << "** [dispatcher.done@" << context.size << "." << context.rank << "]" << std::endl; }
                access.unlock();
                return;
            }

            //------------------------------------------------------------------
            //
            // engage this thread in computation
            //
            //------------------------------------------------------------------
            --ready;
            while(jobs.size>0)
            {

                // take the job
                jnode *j = jobs.pop_front();
                if(verbose) { std::cerr << "** [dispatcher.take@" << context.size << "." << context.rank << "] => job#" << j->uuid << " out of " << jobs.size << " remaining jobs" << std::endl; }

                // let other threads work
                access.unlock();
                try
                {
                    j->call(context,access);
                    access.lock();
                    junk.store(j);
                }
                catch(...)
                {
                    access.lock();
                    jerr.store(j);
                }

            }

            //------------------------------------------------------------------
            //
            // mutex is locked at end of computation
            //
            //------------------------------------------------------------------
            ++ready;
            if(ready>=workers.count)
            {
                synch.broadcast();
            }
            goto CYCLE;


        }

    }
}
