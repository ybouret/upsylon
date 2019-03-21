
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
            // direct suppression of pending jobs
            {
                Y_LOCK(workers.access);
                done=true;
                while( jobs.size )
                {
                    jnode *j = jobs.pop_back();
                    destruct(j);
                    object::release1(j);
                }
            }

            // wait for current jobs to end
            join();
            assert(0==jobs.size);

            // and final clear
            jmem.clear();

        }


        dispatcher:: dispatcher(const bool v) :
        jobs(),
        jmem(),
        workers(v),
        done(false),
        verbose( workers.verbose )
        {
            // and start
            workers.run(start,this);
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
            
        }

        void dispatcher:: start(void *addr, parallel &context, lockable &access)
        {
            assert(addr);
            dispatcher &self = *static_cast<dispatcher *>(addr);
            self.loop(context,access);
        }

        void dispatcher:: loop(parallel &context, lockable &access)
        {
            Y_LOCK(access);
            std::cerr << "dispatcher.loop@" << context.size << "." << context.rank << std::endl;
        }

    }
}
