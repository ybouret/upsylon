#include "y/oxide/types.hpp"
#include "y/randomized/bits.hpp"

namespace upsylon
{
    namespace Oxide
    {
        Coord1D CoordOps:: GetNatural( const unit_t m, randomized::bits &ran ) throw()
        {
            const unit_t ma = abs_of(m);
            return ran.range<Coord1D>(0,ma);
        }

        Coord1D CoordOps:: GetInteger( const unit_t m, randomized::bits &ran ) throw()
        {
            const unit_t ma = abs_of(m);
            return ran.range<Coord1D>(-ma,ma);
        }

        Coord1D CoordOps:: GetWithin( const Coord1D a, const Coord1D b, randomized::bits &ran) throw()
        {
            return ran.range<Coord1D>(a,b);
        }

    }

}

