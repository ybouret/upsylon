
#include "y/gfx/mask.hpp"

namespace upsylon
{

    namespace graphic
    {

        mask:: mask() : mask_() {}

        mask:: ~mask() throw()
        {
        }

        void mask:: insert(const coord p)
        {
            const coord    tmp = coord_to_key(p);
            const uint8_t *key = (const uint8_t *)&tmp;
            (void) insert_by(key, sizeof(coord), p);
        }

        coord mask:: coord_to_key(const coord &p) throw()
        {
            return coord( swap_be_as(p.x), swap_be_as(p.y) );
        }



    }

}
