
#include "y/concurrent/task/pipeline.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/aliasing.hpp"

#include <iomanip>

namespace upsylon
{
    namespace concurrent
    {

        pipeline:: engine:: engine(executable   &user_host,
                                   const size_t  user_size,
                                   const size_t  user_rank) :
        object(),
        executable::launcher(user_host,user_size,user_rank),
        inode<engine>()
        {
        }


        pipeline:: engine:: ~engine() throw()
        {
        }


        static const char pfx[] = "[pipe.";

        pipeline:: ~pipeline() throw()
        {
            cleanup();
        }



        pipeline:: pipeline() :
        executable(),
        waiting(),
        working(),
        pending(),
        running(),
        shallow(),
        ready(0),
        start(),
        built(false),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_PIPELINE) )
        {
            setup();
        }


        void pipeline:: trim() throw()
        {
            Y_LOCK(access);
            while( shallow.size )
            {
                task *t = shallow.pop_front();
                object::release1(t);
            }
        }

        void pipeline:: free() throw()
        {
            Y_LOCK(access);
            while(pending.size)
            {
                task *t = pending.pop_back();
                self_destruct(*t);
                shallow.push_back(t);
            }

        }

        void pipeline:: cleanup() throw()
        {
            access.lock();
            Y_PIPELINE_LN(pfx<<"----] #" << topo->size() << " finishing ----");

            while(pending.size)
            {
                task *t = pending.pop_back();
                self_destruct(*t);
                object::release1(t);
            }

            trim();
            

            Y_PIPELINE_LN(pfx<<"quit] #" << topo->size() );
            start.broadcast();
            access.unlock();


        }

        void pipeline:: setup()
        {
            const size_t count = topo->size();
            try
            {
                //--------------------------------------------------------------
                //
                // building threads
                //
                //--------------------------------------------------------------
                {
                    Y_PIPELINE_LN(pfx<<"init] #" << topo->size() );
                    size_t rank = 0;
                    while(rank<count)
                    {
                        engine *eng = new engine(*this,count,rank);
                        waiting.push_back(eng);
                        Y_MUTEX_PROBE(access,ready>rank);
                        ++rank;
                    }
                    Y_PIPELINE_LN(pfx<<"made] #" << topo->size() );
                }

                //--------------------------------------------------------------
                //
                // thread placement
                //
                //--------------------------------------------------------------
                {
                    assert(waiting.size==topo->nodes.size);
                    const topology::node *cpu = topo->nodes.head;
                    engine               *eng = waiting.head;
                    for(size_t n=count;n>0;--n,cpu=cpu->next,eng=eng->next)
                    {
                        const char *who = verbose ? eng->label : NULL;
                        nucleus::thread::assign(eng->handle,cpu->rank,who);
                    }
                }

                //--------------------------------------------------------------
                //
                // all built
                //
                //--------------------------------------------------------------
                built = true;
                Y_PIPELINE_LN(pfx<<"----] #" << topo->size() << " synchronized ----" );

            }
            catch(...)
            {
                start.broadcast();
                Y_MUTEX_PROBE(access,ready<=0);
                throw;
            }
        }


        void pipeline:: call(const context &ctx) throw()
        {
            //------------------------------------------------------------------
            //
            // LOCK
            //
            //------------------------------------------------------------------

            access.lock();
            ++ready;
            Y_PIPELINE_LN(pfx<<"<ok>] @ " << ctx.label << " (ready = " << std::setw(ctx.setw) << ready << "/" << ctx.size << ")" );

            //------------------------------------------------------------------
            //
            // wait on a LOCKED mutex
            //
            //------------------------------------------------------------------
            start.wait(access);

            //------------------------------------------------------------------
            //
            // awake on a LOCKED mutex
            //
            //------------------------------------------------------------------
            if(!built)
            {
                // emergency exit
                Y_PIPELINE_LN(pfx<<"err!] @"<<ctx.label);
                --ready;
                access.unlock();
                return;
            }

            //------------------------------------------------------------------
            //
            // first valid wake up :)
            //
            //------------------------------------------------------------------
        LOOP:
            //Y_PIPELINE_LN(pfx<<"call] @" << ctx.label);
            if(pending.size>0)
            {
                //--------------------------------------------------------------
                //
                // working phase
                //
                //--------------------------------------------------------------

                // get task
                tasks &io   = aliasing::_(running);
                task  *todo = io.push_back( pending.pop_front() );
                Y_PIPELINE_LN(pfx<<"run+] @" << ctx.label << " job#" << todo->uuid);

                // run UNLOCKED
                access.unlock();
                todo->code(access);
                access.lock();

                // done
                Y_PIPELINE_LN(pfx<<"run-] @" << ctx.label << " job#" << todo->uuid);
                store_task( io.unlink(todo) );

                start.wait(access);
                goto LOOP;
            }
            else
            {
                //--------------------------------------------------------------
                //
                // wake-up on empty queue => return!
                //
                //--------------------------------------------------------------
                Y_PIPELINE_LN(pfx<<"done] @" << ctx.label);
            }


            access.unlock();
        }


        pipeline:: task:: task(const job::uuid  U,
                               const job::type &J) :
        next(0), prev(0), uuid(U), code(J), priv(0)
        {
        }

        pipeline:: task:: ~task() throw()
        {
            assert(0==next);
            assert(0==prev);
        }


        pipeline::task * pipeline:: query_task(const job::type &J)
        {
            // assuming locked
            task *t = shallow.size ? shallow.pop_back() : object::acquire1<task>();
            try {
                new (t) task(jid,J);
                ++jid;
                return t;
            }
            catch(...)
            {
                t->next = t->prev = 0;
                shallow.push_back(t);
                throw;
            }
        }

        void pipeline:: store_task(task *t) throw()
        {
            assert(t);
            self_destruct(*t);
            shallow.push_back(t);
        }


        job::uuid pipeline:: enqueue( const job::type &J )
        {
            Y_LOCK(access);

            task *t = pending.push_back( query_task(J) );
            Y_PIPELINE_LN(pfx<<"load] job#" << t->uuid);

            start.signal();
            return t->uuid;
        }
    }

}


