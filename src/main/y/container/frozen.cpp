#include "y/container/frozen.hpp"
#include "y/object.hpp"
#include "y/type/block/zset.hpp"
#include "y/code/base2.hpp"
#include "y/type/utils.hpp"
#include <cstring>

namespace upsylon
{
    
    frozen_:: ~frozen_() throw()
    {
        assert(block_size>0);
        assert(is_a_power_of_two(block_size));
        assert(block_addr!=NULL);
        
        memset(block_addr,0,block_size);
        object:: operator delete(block_addr,block_size);
        _bzset(block_addr);
        _bzset(block_size);
    }
    
    static inline size_t frozen_block_size(const size_t usr_size) throw()
    {
        static const size_t min_size = sizeof(double);
        return next_power_of_two( max_of(min_size,usr_size) );
    }
    
    frozen_:: frozen_(const size_t usr_size) :
    block_size( frozen_block_size(usr_size)       ),
    block_addr( object:: operator new(block_size) )
    {
    }
    

}
