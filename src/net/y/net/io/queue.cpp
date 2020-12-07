
#include "y/net/io/queue.hpp"
#include "y/type/aliasing.hpp"

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


        void io_queue:: load(size_t bs)
        {
            assert(bs<=data.length());
            if(bs>pool.size) reserve(bs-pool.size); assert(pool.size>=bs);

            uint8_t *bp = data.as<uint8_t>();
            while(bs-- > 0 )
            {
                assert(pool.size>0);
                aliasing::_( push_back( pool.pop_back() )->code ) = *(bp++);
            }

        }

    }

}


