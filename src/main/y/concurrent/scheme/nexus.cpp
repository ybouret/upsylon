
#include "y/concurrent/scheme/nexus.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace concurrent
    {

        nexus:: jnode:: ~jnode() throw()
        {
        }

        nexus:: jnode:: jnode(const job_uuid u, const job_type &J) :
        next(0), prev(0), uuid(u), call(J), valid(1)
        {
        }

        nexus::jnode * nexus::jnode:: destruct() throw()
        {
            self_destruct(call);
            _bzset(uuid);
            _bzset(valid);
            return this;
        }
        
    }

}

namespace upsylon
{
    namespace concurrent
    {
        nexus:: jpool:: jpool() throw()  : jpool_type()
        {
            assert(!head);
            assert(0==size);
        }

        nexus:: jpool::  ~jpool() throw()
        {
            clear();
        }

        nexus::jnode * nexus:: jpool:: fetch()
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

        void nexus:: jpool:: clear() throw()
        {
            while(size)
            {
                assert(head);
                jnode *j = checked( query() );
                object::release1(j);
            }
        }

        void nexus:: jpool:: gc() throw()
        {
            for( jnode *j = head; j; j=checked(j)->next)
                ;
        }


        nexus::jnode * nexus:: jpool:: checked( jnode *j ) throw()
        {
            assert(j);
            return (j->valid) ? j->destruct() : j;
        }

    }

}

namespace upsylon
{
    namespace concurrent
    {



        nexus:: nexus(const bool v) :
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
                std::cerr << "|_[nexus.prepared]" << std::endl;
            }
        }

        executor & nexus:: engine() throw()
        {
            return workers;
        }


        void nexus:: remove_pending() throw()
        {
            Y_LOCK(access);
            while( pending.size )
            {
                assert(pending.tail);
                assert(pending.tail->valid);
                storage.store( pending.pop_back()->destruct() );
                assert(storage.head);
                assert(!storage.head->valid);
            }
        }

        void nexus:: reserve_jobs( size_t n )
        {
            Y_LOCK(access);
            while(n-- > 0)
            {
                storage.store( object::acquire1<jnode>() );
                assert(storage.head);
                assert(!storage.head->valid);
            }
        }

        double nexus:: efficiency( const double speed_up ) const
        {
            return workers[0].efficiency(speed_up);
        }




        job_uuid nexus:: enqueue( const job_type &job )
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
                _bzset(j->valid); // mark as not constructed in any case
                storage.store(j);
                throw;
            }
        }

        void nexus:: process(array<job_uuid> &uuids, const array<job_type> &batch)
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
                    _bzset(j->valid); // mark as not constructed in any case
                    storage.store(j);
                    throw;
                }
            }
            activity.signal();
        }


        void nexus:: flush() throw()
        {
            //__________________________________________________________________
            //
            // take control
            //__________________________________________________________________
            access.lock();
            if(verbose)
            {
                std::cerr << "|_[nexus.flush request with stopping=<" << (stopping?"true":"false") << ">]" << std::endl;
            }

            if(pending.size>0||current.size>0)
            {
                //______________________________________________________________
                //
                // wait on a locked muted
                //______________________________________________________________
                flushing.wait(access);
                storage.gc();
                
                //______________________________________________________________
                //
                // wake up on a locked mutex
                //______________________________________________________________
                if(verbose)
                {
                    std::cerr << "|_[nexus.flush achieved with stopping=<" << (stopping?"true":"false") << ">]" << std::endl;
                }
            }
            access.unlock();
        }

        void nexus:: start(void     *addr,
                           parallel &context,
                           lockable &)
        {
            assert(addr);
            static_cast<nexus *>(addr)->loop(context);
        }


        nexus:: ~nexus() throw()
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
                std::cerr << "|_[nexus.quit: remaining #jobs=" << pending.size  << "]" << std::endl;
            }

            //__________________________________________________________________
            //
            //  suppression of pending jobs
            //__________________________________________________________________
            while( pending.size )
            {
                jnode *j = pending.pop_back()->destruct();
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

        void nexus:: loop(parallel &context)
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
                std::cerr << "|_[nexus.loop@" << context.label << "]" << std::endl;
            }
            const size_t count = workers.num_threads();
            assert(prepared<count);
            ++prepared;
            if(verbose&&prepared>=count)
            {
                std::cerr << "|_[nexus.synchronized]" << std::endl;
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
                if(verbose) { std::cerr << "|_[nexus.done@" << context.label << "]" << std::endl; }
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
                if(verbose) { std::cerr << "|_[nexus.call@"<< context.label << ": job#" << j->uuid << "]" << std::endl; }

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
