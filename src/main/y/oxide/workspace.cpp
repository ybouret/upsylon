#include "y/oxide/workspace.hpp"
#include "y/exception.hpp"


namespace upsylon
{
    namespace Oxide
    {

        void __Workspace:: CheckLocalSizes( const Coord1D *sizes, const unsigned dim )
        {
            assert(sizes);
            if(dim<=0||dim>3) throw exception("Invalid Workspace dimension=%u",dim);
            for(size_t i=0;i<dim;++i)
            {
                if(sizes[i]<0) throw exception("Workspace.sizes#%u=%ld",dim,long(sizes[i]));
            }
        }
    }
}

