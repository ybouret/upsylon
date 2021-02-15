

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
        halting(true),
        kcode(0),
        kdata(0),
        verbose( query_threads_verbosity() )
        {
            initialize();
        }
        
        

        void crew:: initialize()
        {
            assert(halting==true);
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
                
                
            }
            catch(...)
            {
                assert(halting);
                start.broadcast();
                Y_MUTEX_PROBE(synchronize,ready<=0);
                throw;
            }
            
        }
        
        crew:: ~crew() throw()
        {
            {
                Y_LOCK(synchronize);
                if(!halting)
                {
                    halting = true;
                    start.broadcast();
                }
            }
            
#if 0
            {
                Y_LOCK(synchronize);
                Y_CREW_PRINTLN(pfx << ".halt] " << topo->size() << " worker" << textual::plural_s(topo->size()) );
                halting=true;
            }
            start.broadcast();
            Y_MUTEX_PROBE(synchronize,ready<=0);
#endif
        }
        
        void crew:: entry_stub(void *args) throw()
        {
            assert(args);
            static_cast<crew *>(args)->entry();
        }
        
        void crew:: entry() throw()
        {
            synchronize.lock();
            const worker &agent = squad.back();
            Y_CREW_PRINTLN(pfx<<".run!] "<< agent.label);
            ++ready;
            
            // wait on a LOCKED mutex
            start.wait(synchronize);
            
            // wake up on a LOCKED mutex
            Y_CREW_PRINTLN(pfx<<".wake] "<< agent.label);

            if(halting)
            {
                Y_CREW_PRINTLN(pfx<<".nope] "<< agent.label);
                --ready;
                synchronize.unlock();
                return;
            }
            
            // not halting, perform the code
            Y_CREW_PRINTLN(pfx<<".call] "<< agent.label);
            assert(kcode);
            synchronize.unlock();
            
            kcode(kdata,agent,synchronize);
            
            {
                Y_LOCK(synchronize);
                assert(ready>0);
                Y_CREW_PRINTLN(pfx<<".quit] "<< agent.label);
                --ready;
            }
         }
        
        void crew:: run(executable code, void *data)
        {
            assert(code);
            {
                Y_LOCK(synchronize);
                kcode   = code;
                kdata   = data;
                halting = false;
                std::cerr << "will run halting=" << halting << std::endl;
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

