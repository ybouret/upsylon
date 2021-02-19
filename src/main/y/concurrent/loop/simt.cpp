
#include "y/concurrent/loop/simt.hpp"
#include "y/type/aliasing.hpp"
#include <iomanip>



namespace upsylon
{

    namespace concurrent
    {



        
        //----------------------------------------------------------------------
        //
        // setup simt
        //
        //----------------------------------------------------------------------
        static const char pfx[] = "[simt";
        
        simt:: simt() :
        cycles(0),
        code(NULL),
        ready(0),
        cycle(),
        joined(0),
        finish(),
        crew( topo->size()   ),
        built(false),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_SIMT) )
        {
            setup();
        }

        


        void simt:: setup()
        {
            try
            {
                //--------------------------------------------------------------
                // create threads with first syncrhonization
                //--------------------------------------------------------------
                const size_t count = topo->size();
                Y_SIMT_LN(pfx<<".init] #" << count);
                for(size_t rank=0;rank<count;++rank)
                {
                    crew.build<simt&,size_t,size_t>(*this,count,rank);
                    Y_MUTEX_PROBE(access,ready>rank);
                }
                Y_SIMT_LN(pfx<<".made] #" << count);
                
                //--------------------------------------------------------------
                // first wait on cycle, place threads
                //--------------------------------------------------------------
                {
                    size_t rank = 0;
                    for(const topology::node *node=topo->nodes.head;node;node=node->next,++rank)
                    {
                        const thread &thr = crew[rank];
                        const char   *who = verbose ? thr.label : NULL;
                        nucleus::thread::assign(thr.handle,node->rank,who);
                    }
                }
                
                
                //--------------------------------------------------------------
                // first wait on cycle
                //--------------------------------------------------------------
                Y_SIMT_LN(pfx<<".----] #" << count << " synchronized");

                built = true;

            }
            catch(...)
            {
                cycle.broadcast();
                Y_MUTEX_PROBE(access,ready<=0);
                throw;
            }
            
        }

        //----------------------------------------------------------------------
        //
        // cleanup
        //
        //----------------------------------------------------------------------
        
        simt:: ~simt() throw()
        {
            cleanup();
        }
        
        
        void simt:: cleanup() throw()
        {
            {
                Y_LOCK(access);
                Y_SIMT_LN(pfx<<".quit] #" << crew.size() );
            }
            cycle.broadcast();
        }
        
        //----------------------------------------------------------------------
        //
        // main loop
        //
        //----------------------------------------------------------------------
        
        void simt:: call(const context &ctx) throw()
        {
            //------------------------------------------------------------------
            //
            // LOCK access
            //
            //------------------------------------------------------------------
            access.lock();
            const size_t count = topo->size();
            ++ready;
            Y_SIMT_LN(pfx<<".<ok>] @ " << ctx.label << " (ready = " << std::setw(ctx.setw) << ready << "/" << count << ")" );
            
            //------------------------------------------------------------------
            //
            // waiting on a LOCKED access
            //
            //------------------------------------------------------------------
            cycle.wait(access);
            
            //------------------------------------------------------------------
            //
            // wake up on a LOCKED access
            //
            //------------------------------------------------------------------
            
            // first cycle!
            if(!built)
            {
                Y_SIMT_LN(pfx<<".err!] @" << ctx.label);
                --ready;
                access.unlock();
                return;
            }

            //------------------------------------------------------------------
            //
            // main loop
            //
            //------------------------------------------------------------------

        CYCLE:
            if(code)
            {
                assert(ready>0);
                Y_SIMT_LN(pfx<<".run!] @" << ctx.label);

                // run UNLOCKED
                access.unlock();
                code->run(ctx,access);
                //...

                // post-synchronization on a LOCKED access
                access.lock();
                if(++joined>count)
                {
                    Y_SIMT_LN(pfx<<".done] @" << ctx.label << " (joined=all) ");
                    finish.broadcast();
                }
                else
                {
                    Y_SIMT_LN(pfx<<".wait] @" << ctx.label << " (joined=" << joined  <<")");
                    finish.wait(access);
                }


                Y_SIMT_LN(pfx<<".next] @" << ctx.label);
                --ready;
                cycle.wait(access);
                goto CYCLE;
            }

            // this is the end
            assert(ready>0);
            Y_SIMT_LN(pfx<<".bye!] @" << ctx.label);
            --ready;
            access.unlock();
            return;
        }


        //----------------------------------------------------------------------
        //
        // interacting with main loop
        //
        //----------------------------------------------------------------------
        void simt:: loop(runnable &obj)
        {
            Y_LOCK(access);
            assert(!code);
            assert(topo->size() == ready);
            ++aliasing::_(cycles);
            Y_SIMT_LN(pfx<<".loop] <cycle " << cycles << ">");
            code  = &obj;
            cycle.broadcast();
        }


        void simt:: join() throw()
        {
            access.lock();
            assert(code);
            const size_t count = topo->size();
            if(++joined>count)
            {
                Y_SIMT_LN(pfx<<".done] @primary (joined=all)");
                finish.broadcast();
            }
            else
            {
                Y_SIMT_LN(pfx<<".wait] @primary (joined=" << joined << ")" );
                finish.wait(access);
            }
            joined = 0;
            code   = 0;
            access.unlock();

            // re-sync
            Y_MUTEX_PROBE(access,ready<=0);
            ready  = count;
            Y_SIMT_LN(pfx<<".join] <cycle " << cycles << ">");
        }
    }

}

