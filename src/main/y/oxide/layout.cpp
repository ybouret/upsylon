
#include "y/oxide/layout.hpp"

namespace upsylon
{
    namespace  Oxide
    {
        LayoutInfo:: LayoutInfo(const size_t d) throw() :
        dimension(d),
        items(0)
        {
            assert(dimension>=1&&dimension<=3);
        }

        LayoutInfo:: ~LayoutInfo() throw()
        {
        }

        LayoutInfo:: LayoutInfo( const LayoutInfo &other ) throw() :
        dimension( other.dimension ),
        items( other.items )
        {
        }
        

    }
}
