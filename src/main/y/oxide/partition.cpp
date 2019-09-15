#include "y/oxide/partition.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {
        size_t __Partition:: Count( const Coord1D *sizes, const unsigned dims)
        {
            assert(sizes);
            assert(dims>=1);
            assert(dims<=3);
            size_t p = 1;
            for(unsigned i=0;i<dims;++i)
            {
                const Coord1D sz = sizes[i];
                if( sz <=0 ) throw exception("Oxide::Partition(invalid sizes[%u])", i);
                p *= sz;
            }
            return p;
        }
    }
}


