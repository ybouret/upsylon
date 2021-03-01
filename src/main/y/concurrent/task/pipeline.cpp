
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
        
        pipeline:: ~pipeline() throw()
        {
            // remove extra work
            {
                Y_LOCK(access);
                Y_PIPELINE_LN(pfx << "quit] <#" << topo->size() << ">  with #todo=" << todo.size);
                leave = true;
                todo.release();
            }
            
            // flush current tasks
            flush();
            
            // shutdown all crew
            finish();
        }
        
        
        
        pipeline::pipeline() :
        component(),
        crew(),
        busy(),
        todo(),
        proc(),
        done(),
        ready(0),
        leave(false),
        verbose(nucleus::thread::verbosity(Y_VERBOSE_THREADS))
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
            Y_PIPELINE_LN(pfx << "make] <#" << topo->size() << ">  --------");
            const size_t  count = topo->size();
            const size_t &rank  = crew.size;
            try
            {
                while (rank < count)
                {
                    crew.push_back(new worker(*this, count, rank));
                }
                Y_MUTEX_PROBE(access, ready >= count);
                
            }
            catch (...)
            {
                finish();
                throw;
            }
            
            Y_PIPELINE_LN(pfx << "made] <#" << topo->size() << "/> --------");
        }
        
        unsigned  pipeline:: status() const throw()
        {
            unsigned      ans  = DONE;
            if(todo.size) ans |= TODO;
            if(busy.size) ans |= BUSY;
            return ans;
        }

        

        void pipeline:: deactivate_busy(worker *replica) throw()
        {
            assert(replica);
            assert(busy.owns(replica));
            assert(replica->deal);
            assert(proc.owns(replica->deal));
            
            done.cancel(proc.unlink(replica->deal)); // cancel this deal
            replica->deal = NULL;                    // no more deal here
            crew.push_back(busy.unlink(replica));    // send 'myself' back to crew
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
                // perform assigned contract
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
                Y_PIPELINE_LN(pfx << "--->] todo: " << todo.size << " busy: " << busy.size);
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
                        Y_PIPELINE_LN(pfx << "--->] @" << current->label << " : recycle");
                        goto RECYCLE;
                    }
                    else
                    {
                        // standby!
                        Y_PIPELINE_LN(pfx << "--->] @" << replica->label << " : standby/todo");
                        goto STANDBY;
                    }
                }
                else
                {
                    if(busy.size>0)
                    {
                        Y_PIPELINE_LN(pfx << "--->] @" << replica->label << " : standby/busy");
                        goto STANDBY;
                    }
                    else
                    {
                        Y_PIPELINE_LN(pfx << "--->] @" << replica->label << " : standby/flushed");
                        goto STANDBY;
                    }
                }



            }
            
            //LEAVE:
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
            Y_PIPELINE_LN(pfx << "+job] $" << todo.tail->uuid);
            
            
            //------------------------------------------------------------------
            //
            // dispatch the job(s)...
            //
            //------------------------------------------------------------------
            size_t num = min_of<size_t>(todo.size,crew.size);
            Y_PIPELINE_LN(pfx << "load] #" << num << " (primary)");
            while (num-- > 0)
            {
                activate_worker()->broadcast();
            }

            //------------------------------------------------------------------
            //
            // auto unlock, ready for more yield or flush
            //
            //------------------------------------------------------------------
            return U;
        }
        
        
        
        void pipeline::flush() throw()
        {
            Y_LOCK(access);
            if (busy.size)
            {
                Y_PIPELINE_LN(pfx << "^^^^] #busy=" << busy.size);
            }
            
            Y_PIPELINE_LN(pfx << "----] flushed");
        }
        
        
        
    }
    
}


