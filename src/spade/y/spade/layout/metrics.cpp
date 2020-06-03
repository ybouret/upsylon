#include "y/spade/layout/metrics.hpp"
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
        
        LayoutMetrics:: LayoutMetrics(const LayoutMetrics &L) throw() :
        dimensions(L.dimensions),
        items(L.items)
        {
        }
        
        bool LayoutMetrics:: _isThick(const Coord1D *w) const throw()
        {
            assert(w);
            for(unsigned i=0;i<dimensions;++i)
            {
                assert(w[i]>0);
                if(w[i]<=1) return false;
            }
            return true;
        }

        
    }
}

#include "y/parops.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade {
        
        void LayoutMetrics:: Split1D(Coord1D       &length,
                                     Coord1D       &offset,
                                     const Coord1D  size,
                                     const Coord1D  rank,
                                     const unsigned dim)
        {
            assert(length>0);
            assert(size>0);
            assert(rank>=0);
            assert(rank<size);
            parops::split_any(length, offset, size, rank);
            if(length<=0) throw exception("Spade::Layout::Split1D(too many ranks in dim#%u!)",dim);
        }
        
    }
    
}
