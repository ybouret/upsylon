
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
            //__________________________________________________________________
            //
            // and start
            //__________________________________________________________________
            workers.run(start,this);

            //__________________________________________________________________
            //
            // wait for initial synchro
            //__________________________________________________________________
            const size_t count = workers.num_threads();
            Y_MUTEX_PROBE(access,prepared>=count);

            if(verbose)
            {
                std::cerr << "|_[dispatcher.prepared]" << std::endl;
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

        double dispatcher:: efficiency( const double speed_up ) const
        {
            return workers[0].efficiency(speed_up);
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

        void dispatcher:: process(array<job_uuid> &uuids, const array<job_type> &batch)
        {
            assert(uuids.size()==batch.size());
            Y_LOCK(access);
            const size_t n = batch.size();
            for(size_t i=1;i<=n;++i)
            {
                jnode *j = storage.fetch();
                try
                {
                    new (j) jnode( (uuids[i]=uuid) ,batch[i]);
                    ++uuid;
                    pending.push_back(j);

                }
                catch(...)
                {
                    (int&)(j->valid)=0; // mark as not constructed in any case
                    storage.store(j);
                    throw;
                }
            }
            activity.signal();
        }


        void dispatcher:: flush() throw()
        {
            //__________________________________________________________________
            //
            // take control
            //__________________________________________________________________
            access.lock();
            if(verbose)
            {
                std::cerr << "|_[dispatcher.flush request with stopping=<" << (stopping?"true":"false") << ">]" << std::endl;
            }

            if(pending.size>0||current.size>0)
            {
                //______________________________________________________________
                //
                // wait on a locked muted
                //______________________________________________________________
                flushing.wait(access);

                //______________________________________________________________
                //
                // wake up on a locked mutex
                //______________________________________________________________
                if(verbose)
                {
                    std::cerr << "|_[dispatcher.flush achived with stopping=<" << (stopping?"true":"false") << ">]" << std::endl;
                }
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
            //__________________________________________________________________
            //
            // take control
            //__________________________________________________________________
            access.lock();

            //__________________________________________________________________
            //
            // next activity will be to comeback
            //__________________________________________________________________
            stopping=true;
            if(verbose)
            {
                std::cerr << "|_[dispatcher.quit: remaining #jobs=" << pending.size  << "]" << std::endl;
            }

            //__________________________________________________________________
            //
            //  suppression of pending jobs
            //__________________________________________________________________
            while( pending.size )
            {
                jnode *j = pending.pop_back();
                destruct(j);
                object::release1(j);
            }

            //__________________________________________________________________
            //
            // flush current threads
            //__________________________________________________________________
            access.unlock();
            flush();

            //__________________________________________________________________
            //
            // prepare for final call
            //__________________________________________________________________
            access.lock();
            activity.broadcast();
            access.unlock();

            //__________________________________________________________________
            //
            // wait for final synchro
            //__________________________________________________________________
            Y_MUTEX_PROBE(access,prepared<=0);

            assert(0==pending.size);
            assert(0==current.size);
            //__________________________________________________________________
            //
            // and memory is releaded by storage and aborted
            //__________________________________________________________________

        }

        void dispatcher:: loop(parallel &context)
        {
            //==================================================================
            //
            //
            // initialisation
            //
            //
            //==================================================================
            access.lock();
            if(verbose)
            {
                std::cerr << "|_[dispatcher.loop@" << context.label << "]" << std::endl;
            }
            const size_t count = workers.num_threads();
            assert(prepared<count);
            ++prepared;
            if(verbose&&prepared>=count)
            {
                std::cerr << "|_[dispatcher.synchronized]" << std::endl;
            }


            //==================================================================
            //
            //
            // loop to wait for being woken up
            //
            //
            //==================================================================
        WAIT_FOR_ACTIVITY:
            //------------------------------------------------------------------
            //
            // wait on a LOCKED muted
            //
            //------------------------------------------------------------------
            activity.wait(access);

            //------------------------------------------------------------------
            //
            // wake up on a LOCKED mutes
            //
            //------------------------------------------------------------------
            if(stopping)
            {
                if(verbose) { std::cerr << "|_[dispatcher.done@" << context.label << "]" << std::endl; }
                assert(prepared>0);
                --prepared;
                access.unlock();
                return;
            }

            //==================================================================
            //
            //
            // work detection: LOCKED at this point
            //
            //
            //==================================================================
            while(pending.size>0)
            {
                //--------------------------------------------------------------
                //
                // take the job
                //
                //--------------------------------------------------------------
                jnode *j = pending.pop_front();
                current.push_back(j);
                if(verbose) { std::cerr << "|_[dispatcher.call@"<< context.label << ": job#" << j->uuid << "]" << std::endl; }

                //______________________________________________________________
                //
                // set activity for other
                //______________________________________________________________
                if(pending.size>0)
                {
                    activity.signal();
                }

                //______________________________________________________________
                //
                // yield access for other threads to work
                //______________________________________________________________
                access.unlock();

                //______________________________________________________________
                //
                // execute task, UNLOCKED
                //______________________________________________________________
                try
                {
                    j->call(context,access);
                    //...
                    //...
                    //__________________________________________________________
                    //
                    // take control and put back into storage
                    //__________________________________________________________
                    access.lock();
                    storage.store( current.unlink(j) );
                }
                catch(...)
                {
                    //__________________________________________________________
                    //
                    // take control and put back into aborted
                    //__________________________________________________________
                    access.lock();
                    aborted.store( current.unlink(j) );
                }


                //--------------------------------------------------------------
                //
                // LOCKED at this point, ready for next job or exit the loop
                //
                //--------------------------------------------------------------
            }

            //==================================================================
            //
            //
            // end of processing for now, we are LOCKED
            //
            //
            //==================================================================
            if(current.size<=0)
            {
                //______________________________________________________________
                //
                // no more pending tasks, no more pending: signal that we are
                // done so far...
                //______________________________________________________________
                flushing.signal();
            }

            goto WAIT_FOR_ACTIVITY;

        }

    }
}
