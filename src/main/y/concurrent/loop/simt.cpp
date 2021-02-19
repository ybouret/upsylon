
#include "y/concurrent/loop/simt.hpp"
#include <iomanip>



namespace upsylon
{

    namespace concurrent
    {


        size_t simt:: size() const throw()
        {
            return topo->size();
        }

        const context & simt:: operator[](const size_t indx) const throw()
        {
            assert(indx>0);
            assert(indx<=crew.size());
            return *( (&crew.front())+indx );
        }


        //----------------------------------------------------------------------
        //
        // setup simt
        //
        //----------------------------------------------------------------------
        static const char pfx[] = "[simt.";
        
        simt:: simt() :
        code(NULL),
        ready(0),
        cycle(),
        fence(),
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
                Y_SIMT_LN(pfx<<"init] #" << count);
                for(size_t rank=0;rank<count;++rank)
                {
                    assert(ready==rank);
                    crew.build<simt&,size_t,size_t>(*this,count,rank);
                    Y_MUTEX_PROBE(access,ready>rank);
                }
                Y_SIMT_LN(pfx<<"made] #" << count);
                
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
                Y_SIMT_LN(pfx<<"----] #" << count << " synchronized ----");

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
            Y_SIMT_LN(pfx<<"quit] #" << crew.size() );
            assert(NULL==code);
            cycle.broadcast();
            Y_MUTEX_PROBE(access,ready<=0);
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
            Y_SIMT_LN(pfx<<"<ok>] @ " << ctx.label << " (ready = " << std::setw(ctx.setw) << ready << "/" << count << ")" );
            
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
                Y_SIMT_LN(pfx<<"err!] @" << ctx.label);
                --ready;
                access.unlock();
                return;
            }

            //------------------------------------------------------------------
            //
            // main loop: LOCKED
            //
            //------------------------------------------------------------------
        LOOP:
            if(code)
            {
                assert(ready>0);
                // run unlock
                access.unlock();
                code->run(ctx,access);

                // barrier
                access.lock();
                if(--ready<=0)
                {
                    fence.broadcast();
                }
                cycle.wait(access);
                goto LOOP;
            }
            else
            {

                Y_SIMT_LN(pfx<<"bye!] @" << ctx.label);
                assert(ready>0);
                --ready;
                access.unlock();
                return;
            }


        }


        //----------------------------------------------------------------------
        //
        // interacting with main loop
        //
        //----------------------------------------------------------------------
        void simt:: for_each(runnable &obj) throw()
        {
            //
            // auto LOCKED
            //
            Y_LOCK(access);

            assert(!code);
            code  = &obj;
            cycle.broadcast();

            // unleash threads and wait for fence to open
            fence.wait(access);

            // return LOCKED
            assert(ready<=0);
            code  = 0;
            ready = topo->size();

            //
            // auto UNLOCK
            //
        }


    }

}

