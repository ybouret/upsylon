#include "y/spade/topology.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        namespace Kernel
        {

            Topology:: ~Topology() throw()
            {
                _bzset(cores);
            }
            
            Topology:: Topology(const size_t nc) :
            cores(nc)
            {
                if(cores<=0) throw exception("Spade::Topology(no cores)");
            }
            
        }
        
    }
    
}

