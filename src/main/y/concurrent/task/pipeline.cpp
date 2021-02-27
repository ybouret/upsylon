
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
#if 0
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
#endif
        }
        
        
        
        pipeline::pipeline() :
        executable(),
#if 0
        crew(),
        busy(),
        todo(),
        proc(),
        done(),
        ready(0),
        leave(false),
#endif
		verbose(nucleus::thread::verbosity(Y_VERBOSE_THREADS))
        {
            //setup();
        }
        
        
        void pipeline::finish() throw()
        {
#if 0
            for (worker *w = crew.head; w; w = w->next)
            {
                w->broadcast();
            }
            Y_MUTEX_PROBE(access, ready <= 0);
#endif
        }
        
        
        void pipeline::setup()
        {
#if 0
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
#endif
        }
        
        
        
        void pipeline::call(const context &ctx) throw()
        {
            access.lock();
            Y_PIPELINE_LN(pfx << "init] @" << ctx.label);
            
            
            
            access.unlock();
        }
        
        void pipeline::loop(worker *replica) throw()
        {
#if 0
            //const size_t count = topo->size();
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
                done.cancel(proc.unlink(replica->deal));
                replica->deal = NULL;
                crew.push_back(busy.unlink(replica));
                
                //--------------------------------------------------------------
                // check what's next
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
#endif
        }
        
        
        
        job::uuid pipeline::yield(const job::type &J)
        {
#if 0
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
            // dispatch the job(s)
            //
            //------------------------------------------------------------------
            size_t num = min_of<size_t>(todo.size, crew.size);
            Y_PIPELINE_LN(pfx << "load] #" << num);
            while (num > 0)
            {
                --num;
                worker *w = busy.push_back(crew.pop_front()); // get next worker
                w->deal = proc.push_back(todo.pop_front()); // assign work
                w->broadcast();                                 // unleash!
            }
            
            
            
            return U;
#endif
			return 0;
        }
        
        
        
        void pipeline::flush() throw()
        {
#if 0
            Y_LOCK(access);
            if (busy.size)
            {
                Y_PIPELINE_LN(pfx << "^^^^] #busy=" << busy.size);
            }
            
            Y_PIPELINE_LN(pfx << "----] flushed");
#endif
        }
        
        
        
    }
    
}


