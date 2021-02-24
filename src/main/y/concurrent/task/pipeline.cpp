
#include "y/concurrent/task/pipeline.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include <iomanip>

namespace upsylon
{
    namespace concurrent
    {

        pipeline:: task:: ~task() throw()
        {
            assert(0==next);
            assert(0==prev);
            aliasing::_(uuid) = 0;
        }

        pipeline:: task:: task( const job::uuid U, const job::type &J) :
        next(0),
        prev(0),
        uuid(U),
        type(J)
        {
        }

        pipeline::task * pipeline:: query_task(const job::type &J)
        {
            task *t = shallow.size ? shallow.query() : object::acquire1<task>();
            try {

                new (t) task(jid,J);
                ++jid;
                return t;
            }
            catch(...)
            {
                shallow.store(t);
                throw;
            }
        }

        void pipeline:: store_task(task *t) throw()
        {
            assert(t);
            self_destruct(*t);
            shallow.store(t);
        }

        void pipeline:: remove_pending() throw()
        {
            while(pending.size)
            {
                store_task(pending.pop_back());
            }
        }

        void pipeline::   delete_shallow() throw()
        {
            shallow.yield( object::release1<task> );
        }

        static inline void __delete_pending( pipeline::task *t ) throw()
        {
            assert(t);
            self_destruct(*t);
            object::release1(t);
        }

        void  pipeline::  delete_pending() throw()
        {
            pending.yield(__delete_pending);
        }




        static const char pfx[] = "[pipe.";

        pipeline:: ~pipeline() throw()
        {
        }



        pipeline:: pipeline() :
        executable(),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_THREADS) )
        {
        }


        void pipeline:: setup()  
        {
            
        }



        void pipeline:: call(const context &ctx) throw()
        {

            access.lock();
            Y_PIPELINE_LN(pfx<<"init] @" << ctx.label);

            ++ready;

            access.unlock();

        }

        job::uuid pipeline:: yield(const job::type &)
        {
            Y_LOCK(access);

            //------------------------------------------------------------------
            //
            // create the job
            //
            //------------------------------------------------------------------

            return 0;
        }

        void pipeline:: flush() throw()
        {
            Y_LOCK(access);

        }



    }

}


