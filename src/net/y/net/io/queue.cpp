#include "y/net/io/queue.hpp"

namespace upsylon
{
    namespace net
    {

        io_queue:: ~io_queue() throw()
        {
        }

        io_queue:: io_queue(const size_t bs) :
        net_object(),
        io_bytes(),
        data(bs)
        {
            reserve( data.length() );
        }

        size_t io_queue:: block_size() const throw()
        {
            return data.length();
        }



    }

}


