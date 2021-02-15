

#include "y/concurrent/device/crew.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{
    namespace concurrent
    {
        namespace nucleus
        {
            crew:: crew() : topo( new topology() )
            {
            }
            
            crew:: ~crew() throw()
            {
            }
            
        }
        
        static const char pfx[] = "[crew";
        
        crew:: crew() :
        nucleus::crew(),
        squad(topo->size()),
        ready(0),
        start(),
        stall(),
        yoked(0),
        state(anchored),
        kcode(0),
        kdata(0),
        verbose( query_threads_verbosity() )
        {
            initialize();
        }
        
        

        void crew:: initialize()
        {
            assert(anchored==state);
            Y_CREW_PRINTLN(pfx << ".init] " << topo->size() << " worker" << textual::plural_s(topo->size()) );
            
            try
            {
                // create all threads
                const size_t count = topo->size();
                for(size_t rank=0, target=1;rank<count;++rank,++target)
                {
                    squad.build<worker::call,void *,size_t,size_t>(entry_stub,this,count,rank);
                    Y_MUTEX_PROBE(synchronize,ready>=target);
                }
                Y_CREW_PRINTLN(pfx << ".done] " << topo->size() << " worker" << textual::plural_s(topo->size()) );
                
                // thread placement
                {
                    size_t                    rank = 0;
                    for(const topology::node *node = topo->nodes.head;node;node=node->next,++rank)
                    {
                        worker       &mate = squad[rank];
                        const size_t  core = node->rank;
                        Y_CREW_PRINTLN(pfx << ".move] " << mate.label << "@core#" << core);
                        nucleus::thread::assign(mate.handle,core);
                    }
                }
                
                // ok, successfully anchored and placed
                
            }
            catch(...)
            {
                assert(anchored==state);
                start.broadcast();
                Y_MUTEX_PROBE(synchronize,ready<=0);
                throw;
            }
            
        }
        
        crew:: ~crew() throw()
        {
            synchronize.lock();
            Y_CREW_PRINTLN(pfx << ".kill] " << topo->size() << " worker" << textual::plural_s(topo->size()) );
            switch(state)
            {
                case anchored:
                    start.broadcast();
                    synchronize.unlock();
                    Y_MUTEX_PROBE(synchronize,ready<=0);
                    break;
                    
                case launched:
                    if(++yoked>topo->size())
                    {
                        Y_CREW_PRINTLN(pfx << ".done] @primary");
                        stall.broadcast();
                        synchronize.unlock();
                    }
                    else
                    {
                        Y_CREW_PRINTLN(pfx << ".wait] @primary");
                        stall.wait(synchronize);
                        synchronize.unlock();
                    }
                    break;
            }
        }
        
        void crew:: entry_stub(void *args) throw()
        {
            assert(args);
            static_cast<crew *>(args)->entry();
        }
        
        void crew:: entry() throw()
        {
            // LOCK shared mutex
            synchronize.lock();
            const worker &agent = squad.back();
            const size_t  limit = topo->size();
            Y_CREW_PRINTLN(pfx<<".run!] "<< agent.label);
            ++ready;
            
            // wait on a LOCKED mutex
            start.wait(synchronize);
            
            // wake up on a LOCKED mutex
            switch(state)
            {
                case anchored:
                    Y_CREW_PRINTLN(pfx<<".nope] "<< agent.label);
                    --ready;
                    synchronize.unlock();
                    return;
                    
                case launched:
                    Y_CREW_PRINTLN(pfx<<".call] "<< agent.label);
                    assert(kcode);
                    synchronize.unlock();
                    
                    // perform code
                    kcode(kdata,agent,synchronize);
                    
                    // and wait on loked
                    synchronize.lock();
                    if(++yoked>limit)
                    {
                        Y_CREW_PRINTLN(pfx<<".end!]  " << agent.label);
                        stall.broadcast();
                        synchronize.unlock();
                        return;
                    }
                    else
                    {
                        Y_CREW_PRINTLN(pfx<<".wait] " << agent.label);
                        stall.wait(synchronize);
                        synchronize.unlock();
                        return;
                    }
                    
            }
            
            
         }
        
        void crew:: run(executable code, void *data)
        {
            assert(code);
            {
                Y_LOCK(synchronize);
                kcode   = code;
                kdata   = data;
                state   = launched;
                start.broadcast();
            }
        }
        
        
    }
    
}

#include "y/string/env.hpp"

namespace upsylon
{
    namespace concurrent
    {
        bool crew:: query_threads_verbosity()
        {
            if(  environment::flag(Y_VERBOSE_THREADS) )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    }
    
}

