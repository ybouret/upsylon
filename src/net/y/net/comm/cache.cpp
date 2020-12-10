
#include "y/net/comm/cache.hpp"

namespace upsylon
{
    Y_SINGLETON_IMPL(net::comm_cache);
    
    namespace net
    {
        
        comm_cache:: comm_cache() :
        singleton<comm_cache>(),
        memory::dblocks(access)
        {
        }
        
        comm_cache:: ~comm_cache() throw()
        {
        }
        
    }

}


