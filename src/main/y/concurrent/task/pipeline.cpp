
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
            replica->wait(access);
            
            
            //------------------------------------------------------------------
            //
            // wake up on a LOCKED mutex: what do I find ?
            //
            //------------------------------------------------------------------
            if (replica->deal)
            {
                //--------------------------------------------------------------
                // perform contract
                //--------------------------------------------------------------
                Y_PIPELINE_LN(pfx << "call] @" << replica->label << "<$" << replica->deal->uuid << ">");
                assert(proc.owns(replica->deal));
                assert(busy.owns(replica));
                
                {
                    access.unlock();
                    aliasing::_(replica->deal->plan)(access);
                    access.lock();
                }
                
                //--------------------------------------------------------------
                // LOCKED after contract : restore state
                //--------------------------------------------------------------
                Y_PIPELINE_LN(pfx << "done] @" << replica->label << "<$" << replica->deal->uuid << "/>");
                done.cancel(proc.unlink(replica->deal)); // cancel this deal
                replica->deal = NULL;                    // no more deal here
                crew.push_back(busy.unlink(replica));    // send 'myself' back to crew
                
                //--------------------------------------------------------------
                // check status
                //--------------------------------------------------------------
                
                
                
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
                worker *w = busy.push_back(crew.pop_front()); // get next worker
                w->deal   = proc.push_back(todo.pop_front()); // assign work
                w->broadcast();                               // unleash!
            }
            
            // auto unlock, ready for more yield or flush
            
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


