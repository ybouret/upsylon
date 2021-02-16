

#include "y/concurrent/device/crew.hpp"
#include "y/code/textual.hpp"
#include "y/exception.hpp"

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
        kcode(0),
        kdata(0),
        verbose( query_threads_verbosity() )
        {
            on();
        }
        
        

        void crew:: on()
        {
            Y_CREW_PRINTLN(pfx << ".init] #" << topo->size()  );
            try
            {
                //--------------------------------------------------------------
                //
                // create all threads by calling entry()
                // using the probe to ensure proper return
                //
                //--------------------------------------------------------------
                const size_t count = topo->size();
                for(size_t rank=0;rank<count;++rank)
                {
                    squad.build<worker::call,void *,size_t,size_t>(entry_stub,this,count,rank);
                    Y_MUTEX_PROBE(synchronize,ready>rank);
                }
                Y_CREW_PRINTLN(pfx << ".done] #" << topo->size() );

                //--------------------------------------------------------------
                //
                // thread placement, everyone waits on start
                //
                //--------------------------------------------------------------
                {
                    size_t                    rank = 0;
                    for(const topology::node *node = topo->nodes.head;node;node=node->next,++rank)
                    {
                        worker       &mate = squad[rank];
                        const size_t  core = node->rank;
                        Y_CREW_PRINTLN(pfx << ".move] @" << mate.label << "->core#" << core);
                        nucleus::thread::assign(mate.handle,core);
                    }
                }
                

            }
            catch(...)
            {
                //--------------------------------------------------------------
                // emergency exit...
                //--------------------------------------------------------------
                off();
                throw;
            }
            
        }


        void crew:: off() throw()
        {
            synchronize.lock();
            Y_CREW_PRINTLN(pfx << ".kill] " << topo->size() << " worker" << textual::plural_s(topo->size()) );

            if(++yoked>ready)
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

        }

        crew:: ~crew() throw()
        {
            off();
        }
        
        void crew:: entry_stub(void *args) throw()
        {
            assert(args);
            static_cast<crew *>(args)->entry();
        }
        
        void crew:: entry() throw()
        {
            //------------------------------------------------------------------
            //
            // LOCK shared mutex
            //
            //------------------------------------------------------------------
            synchronize.lock();
            const worker &agent = squad.back();            // newly created
            Y_CREW_PRINTLN(pfx<<".run!] "<< agent.label);
            ++ready;

            //------------------------------------------------------------------
            //
            // wait on a LOCKED mutex and return to setup function
            //
            //------------------------------------------------------------------
            start.wait(synchronize);

            //------------------------------------------------------------------
            //
            // wake-up on a LOCKED mutex
            //
            //------------------------------------------------------------------
            if(kcode)
            {
                Y_CREW_PRINTLN(pfx<<".call] @"<< agent.label);
                synchronize.unlock();
                kcode(kdata,agent,synchronize);
                synchronize.lock();
            }
            else
            {
                Y_CREW_PRINTLN(pfx<<".nope] @"<< agent.label);
            }


            //------------------------------------------------------------------
            //
            // end of loop, use barrier pattern
            //
            //------------------------------------------------------------------
            if(++yoked>ready)
            {
                Y_CREW_PRINTLN(pfx<<".done] @" << agent.label);
                stall.broadcast();
                synchronize.unlock();
                return;
            }
            else
            {
                Y_CREW_PRINTLN(pfx<<".wait] @" << agent.label);
                stall.wait(synchronize);
                synchronize.unlock();
                return;
            }
            
         }
        
        void crew:: run(executable code, void *data)
        {
            assert(code);
            Y_LOCK(synchronize);
            if(kcode) throw exception("concurrent::crew already running!");
            kcode   = code;
            kdata   = data;
            start.broadcast();
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

