#include "y/ipso/ghosts.hpp"

namespace upsylon
{
    namespace ipso
    {
        ghosts:: ~ghosts() throw()
        {
        }

        void ghosts:: ensure( const size_t block_size )
        {
            source.ensure(block_size);
            target.ensure(block_size);
        }

        void ghosts:: local_exchange( field_info &F ) const throw()
        {
            ghost::exchange(F,source,target);
        }

    }
}
