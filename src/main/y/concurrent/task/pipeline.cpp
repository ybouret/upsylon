
#include "y/concurrent/task/pipeline.hpp"
#include "y/type/collapse.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include <iomanip>

namespace upsylon
{
    namespace concurrent
    {
        
        
        static const char pfx[] = "[pipe.";
        
        size_t pipeline:: size() const throw()
        {
            return topo->size();
        }
        
        void pipeline:: display_status() const
        {
            Y_PIPELINE_LN(pfx << "stat] todo: " << todo.size << " | busy: " << busy.size << " | crew: " << crew.size);
        }
        
        double pipeline:: efficiency(const double speed_up) const throw()
        {
            return nucleus::thread::efficiency(speed_up,size());
        }

        
        pipeline:: ~pipeline() throw()
        {
            //------------------------------------------------------------------
            // LOCK access
            //------------------------------------------------------------------
            access.lock();
            Y_PIPELINE_LN(pfx << "quit] <#" << topo->size() << ">  ---------------");
            display_status();
            
            //------------------------------------------------------------------
            // remove extra work
            //------------------------------------------------------------------
            todo.release();

            //------------------------------------------------------------------
            // flush current tasks
            //------------------------------------------------------------------
            if(busy.size>0)
            {
                Y_PIPELINE_LN(pfx << " ...] final flush!");
                flushed.wait(access);
            }
            
            //------------------------------------------------------------------
            // all done: UNLOCK
            //------------------------------------------------------------------
            Y_PIPELINE_LN(pfx << "bye!] <#" << topo->size() << "/> ---------------");
            access.unlock();

            //------------------------------------------------------------------
            // shutdown all crew
            //------------------------------------------------------------------
            finish();
        }
        
        
        
        pipeline::pipeline() :
        component(),
        flushed(),
        crew(),
        busy(),
        todo(),
        proc(),
        done(),
        ready(0),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_THREADS) )
        {
            setup();
        }
        
        
        void pipeline::finish() throw()
        {
            for (worker *w = crew.head; w; w = w->next)
            {
                w->broadcast();
            }
            Y_MUTEX_PROBE(access, ready <= 0);
        }
        
        
        void pipeline::setup()
        {
            Y_PIPELINE_LN(pfx << "make] <#" << topo->size() << ">  ---------------");
            const size_t  count = topo->size();
            const size_t &rank  = crew.size;
            try
            {
                //--------------------------------------------------------------
                // build threads
                //--------------------------------------------------------------
                while (rank<count)
                {
                    crew.push_back(new worker(*this, count, rank));
                }
                
                //--------------------------------------------------------------
                // wait for first sync
                //--------------------------------------------------------------
                Y_MUTEX_PROBE(access, ready >= count);
                
            }
            catch (...)
            {
                finish();
                throw;
            }
            Y_PIPELINE_LN(pfx << "made] <#" << topo->size() << "/> ---------------");
        }
        
        

        

        void pipeline:: deactivate_busy(worker *replica) throw()
        {
            // sanity check
            assert(replica);
            assert(busy.owns(replica));
            assert(replica->deal);
            assert(proc.owns(replica->deal));
            
            // cleanup
            done.cancel(proc.unlink(replica->deal)); // cancel replica's deal
            replica->deal = NULL;                    // mark replica as free
            crew.push_back(busy.unlink(replica));    // send replica back to crew
        }

        // theaded function owned by replica
        void pipeline::loop(worker *replica) throw()
        {
            //------------------------------------------------------------------
            //
            // LOCK access and first sync
            //
            //------------------------------------------------------------------
            access.lock();
            Y_PIPELINE_LN(pfx << "init] @" << replica->label);
            ++ready;
            
            //------------------------------------------------------------------
            //
            // waiting on a LOCKED mutex
            //
            //------------------------------------------------------------------
        STANDBY:
            replica->wait(access);
            
            
            //------------------------------------------------------------------
            //
            // wake up on a LOCKED mutex: what do I find ?
            //
            //------------------------------------------------------------------
            if (replica->deal)
            {
            RECYCLE:
                //--------------------------------------------------------------
                //
                // perform assigned contract, unlocked
                //
                //--------------------------------------------------------------
                Y_PIPELINE_LN(pfx << "call] @" << replica->label << " <$" << replica->deal->uuid << ">");
                assert(proc.owns(replica->deal));
                assert(busy.owns(replica));
                
                {
                    access.unlock();
                    aliasing::_(replica->deal->plan)(access);
                    access.lock();
                }
                
                //--------------------------------------------------------------
                //
                // LOCKED after contract : restore state
                //
                //--------------------------------------------------------------
                Y_PIPELINE_LN(pfx << "done] @" << replica->label << " <$" << replica->deal->uuid << "/>");
                deactivate_busy(replica);


                //--------------------------------------------------------------
                //
                // LOCKED: update status
                //
                //--------------------------------------------------------------
                display_status();
                assert(crew.size>0);
                assert(crew.tail == replica);

                if(todo.size>0)
                {
                    //----------------------------------------------------------
                    // still some work to do: dispatch to other
                    //----------------------------------------------------------
                    {
                        size_t them = crew.size-1;
                        while( (them>0) && (todo.size>0) )
                        {
                            activate_worker()->broadcast();
                            --them;
                        }
                    }

                    //----------------------------------------------------------
                    // work left ? need to recycle this
                    //----------------------------------------------------------
                    if(todo.size)
                    {
                        // recycle!
                        worker *current = activate_worker();
                        assert(replica==current);
                        Y_PIPELINE_LN(pfx << "....] @" << current->label << " --> recycle [todo]");
                        goto RECYCLE;
                    }
                    else
                    {
                        // standby!
                        Y_PIPELINE_LN(pfx << "....] @" << replica->label << " --> standby [todo]");
                        goto STANDBY;
                    }
                }
                else
                {
                    //----------------------------------------------------------
                    // no pending tasks!
                    //----------------------------------------------------------
                    if(busy.size>0)
                    {
                        Y_PIPELINE_LN(pfx << "....] @" << replica->label << " --> standby [busy]");
                        goto STANDBY;
                    }
                    else
                    {
                        Y_PIPELINE_LN(pfx << "....] @" << replica->label << " --> standby [flushed!]");
                        flushed.broadcast();
                        goto STANDBY;
                    }
                }



            }
            
            //------------------------------------------------------------------
            //
            // returning
            //
            //------------------------------------------------------------------
            Y_PIPELINE_LN(pfx << "bye!] @" << replica->label);
            --ready;
            access.unlock();
        }
        
        worker *pipeline:: activate_worker() throw()
        {
            assert(crew.size>0);
            assert(todo.size>0);
            worker *w = busy.push_back(crew.pop_front()); // get next worker
            w->deal   = proc.push_back(todo.pop_front()); // assign next work
            return w;
        }

        void pipeline:: activate_by_primary() throw()
        {
            size_t num = min_of<size_t>(todo.size,crew.size);
            while (num-- > 0)
            {
                activate_worker()->broadcast();
            }
        }
        
        job::uuid pipeline::yield(const job::type &J)
        {
            Y_LOCK(access);
            
            //------------------------------------------------------------------
            //
            // create the job
            //
            //------------------------------------------------------------------
            const job::uuid U = jid;
            todo.append(U, J, done);
            ++jid;
            Y_PIPELINE_LN(pfx << "+job] $" << todo.tail->uuid << " (yield)");
            
            
            //------------------------------------------------------------------
            //
            // dispatch the job(s)...
            //
            //------------------------------------------------------------------
            activate_by_primary();

            //------------------------------------------------------------------
            //
            // auto UNLOCK, ready for more yield or flush
            //
            //------------------------------------------------------------------
            return U;
        }
        
        
        void pipeline:: batch(addressable<job::uuid> &jids, const accessible<job::type> &jobs)
        {
            Y_LOCK(access);
            assert(jids.size()==jobs.size());
            const size_t count = jobs.size();
            size_t       alive = 0;
            try
            {
                for(size_t i=1;alive<count;++alive,++i)
                {
                    const job::uuid U = jid;
                    todo.append(U, jobs[i], done);
                    jids[i] = U;
                    ++jid;
                    Y_PIPELINE_LN(pfx << "+job] $" << todo.tail->uuid << " (batch)" );
                }
                activate_by_primary();
            }
            catch(...)
            {
                while(alive<count)
                {
                    jids[++alive] = 0;
                }
                activate_by_primary();
                throw;
            }
            
        }
        
        
        
        
        void pipeline::flush() throw()
        {
            //------------------------------------------------------------------
            // LOCK
            //------------------------------------------------------------------
            Y_LOCK(access);
            Y_PIPELINE_LN(pfx << "^^^^]");
            display_status();
            
            if (busy.size)
            {
                Y_PIPELINE_LN(pfx<<" -->] waiting for flushed...");
                flushed.wait(access);
            }
            Y_PIPELINE_LN(pfx << ">>>>] flushed");
            //------------------------------------------------------------------
            // UNLOCK
            //------------------------------------------------------------------
        }
        
        
        
    }
    
}


