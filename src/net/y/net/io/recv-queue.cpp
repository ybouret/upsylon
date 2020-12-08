#include "y/net/io/recv-queue.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace net
    {

        void recv_queue:: load(size_t bs)
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

