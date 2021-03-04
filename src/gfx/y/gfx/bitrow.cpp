
#include "y/gfx/bitrow.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace graphic
    {
        bitrow:: ~bitrow() throw()
        {
            p=0;
            _bzset(w);
        }

        bitrow:: bitrow(void        *entry,
                        const size_t width,
                        const zflux &zhost) throw() :
        p(entry),
        w(width),
        z(zhost)
        {
        }

    }

}


