#include "y/spade/layout-metrics.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {
    
    namespace Spade {
     
        LayoutMetrics:: ~LayoutMetrics() throw()
        {
            _bzset(dimensions);
            _bzset(items);
        }
        
        LayoutMetrics:: LayoutMetrics(const unsigned d) throw() :
        dimensions(d),
        items(0)
        {
        }
        
        
    }
}
