
#include "y/memory/small/mt-vein.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            mt_vein:: ~mt_vein() throw()
            {
            }

            mt_vein:: mt_vein( lockable &l, vein &v) throw() :
            hoard(l,v.bytes),
            proxy(v)
            {
            }

            

        }
    }
}
