#include "y/net/comm/queue.hpp"
#include "y/net/comm/cache.hpp"

namespace upsylon
{
    namespace net
    {

        comm_queue:: ~comm_queue() throw()
        {
        }

        static inline
        memory::dblock *query_dblock(const size_t bs)
        {
            static comm_cache &mgr = comm_cache::instance();
            return mgr.query(bs);
        }
        
        comm_queue:: comm_queue(const size_t bs) :
        comm_bytes(),
        data( query_dblock(bs) )
        {
            reserve( data->size );
        }



        void comm_queue:: reset() throw()
        {
            clear();
            reset_();
            data->ldz();
        }




    }

}


