#include "y/net/comm/queue.hpp"

namespace upsylon
{
    namespace net
    {

        comm_queue:: ~comm_queue() throw()
        {
        }

#if 0
        static inline
        memory::dblock *query_dblock(const size_t block_exp2)
        {
            static comm_cache &mgr = comm_cache::instance();
            return mgr.query(block_exp2);
        }
#endif
        comm_queue:: comm_queue(comm_block *block) :
        comm_bytes(),
        data(block)
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


