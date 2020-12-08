#include "y/net/comm/queue.hpp"

namespace upsylon
{
    namespace net
    {

        comm_queue:: ~comm_queue() throw()
        {
        }

        comm_queue:: comm_queue(const size_t bs) :
        comm_bytes(),
        data(bs)
        {
            reserve( data.length() );
        }

        size_t comm_queue:: block_size() const throw()
        {
            return data.length();
        }

        void comm_queue:: reset() throw()
        {
            clear();
            reset_();
        }




    }

}


