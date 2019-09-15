#include "y/oxide/workspace.hpp"
#include "y/exception.hpp"


namespace upsylon
{
    namespace Oxide
    {

        void __Workspace:: CheckLocalSizes(const Coord1D *localSizes,
                                           const unsigned dimensions)
        {
            assert(0!=localSizes);
            if(dimensions<1||dimensions>3) throw exception("Invalid Workspace dimension=%u",dimensions);
            for(unsigned i=0;i<dimensions;++i)
            {
                if(localSizes[i]<0) throw exception("Workspace.sizes#%u=%ld",i,long(localSizes[i]));
            }
        }


        void __Workspace:: CheckBlockTotal( const IOBlock &block, const size_t total )
        {
            if( block.size() != total ) throw exception("IOBlock.size=%lu != %lu!!!", (unsigned long)(block.size()), (unsigned long)total );
        }
 
        

    }
}

