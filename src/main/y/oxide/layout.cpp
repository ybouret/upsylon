
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
            bzset_(dimension);
            bzset_(items);
        }

        LayoutInfo:: LayoutInfo( const LayoutInfo &other ) throw() :
        dimension( other.dimension ),
        items( other.items )
        {
        }




    }
}

#include "y/parops.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace  Oxide
    {
        void LayoutInfo::Split1D( Coord1D &length, Coord1D &offset, const Coord1D sz, const Coord1D rk)
        {
            parops::split_any(length,offset,sz,rk);
            if(length<=0)
            {
                throw exception("Layout too small to split that much!");
            }
        }
    }

}


