
#include "y/spade/partition.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade
    {
        namespace Kernel
        {
#if 0
            Partition:: ~Partition() throw()
            {
                _bzset(cores);
            }
            
            Partition:: Partition(const Coord1D nc) :
            cores( static_cast<size_t>(nc) )
            {
                if(nc<=0) throw exception("Spade::Partition(#cores<=0)");
            }
            
#endif
        }
    }
    
}

