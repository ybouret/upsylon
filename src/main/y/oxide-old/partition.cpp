#include "y/oxide/partition.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {
        size_t __Partition:: Check(const Coord1D *localSizes,
                                   const unsigned dimensions,
                                   const size_t   targetSize)
        {
            assert(0!=localSizes);
            assert(dimensions>=1);
            assert(dimensions<=3);
            size_t p = 1;
            for(unsigned i=0;i<dimensions;++i)
            {
                const Coord1D localSize = localSizes[i];
                if( localSize <=0 ) throw exception("Oxide::Partition(invalid localSizes[%u])", i);
                p *= localSize;
            }
            if(targetSize>0 && p!=targetSize)
            {
                throw exception("Oxide::Partition(sizes mismatch!)");
            }
            return p;
        }
    }
}


