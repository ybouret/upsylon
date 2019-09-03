
#include "y/oxide/field3d.hpp"
#include "y/oxide/field/io.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>


using namespace upsylon;
using namespace Oxide;

template <typename COORD>
static inline
void do_ranks( const COORD &sizes )
{
    COORD org(0);
    COORD top(0);
    for(size_t dim=0;dim<Coord::Get<COORD>::Dimensions;++dim)
    {
        Coord::Of(org,dim) = 0;
        Coord::Of(top,dim) =  Coord::Of(sizes,dim) - 1;
    }
    mloop<Coord1D,COORD> loop(org,top);
    for(loop.start();loop.valid();loop.next())
    {
        const Coord1D rank  = Coord::GlobalRank(sizes,loop.value);
        Y_ASSERT(rank==Coord1D(loop.index-1));
        
        const COORD   ranks = Coord::LocalRanks(rank,sizes);
        Y_ASSERT( ranks == loop.value );
    }
}

Y_UTEST(oxide_ranks)
{
    
    const Coord1D n = 20;
    Coord3D       sizes;
    for(sizes.x=1;sizes.x<=n;++sizes.x)
    {
        do_ranks( sizes.x    );
        for(sizes.y=1;sizes.y<=n;++sizes.y)
        {
            do_ranks( sizes.xy() );
            for(sizes.z=1;sizes.z<=n;++sizes.z)
            {
                do_ranks( sizes );
            }
        }
    }
    
   

}
Y_UTEST_DONE()

