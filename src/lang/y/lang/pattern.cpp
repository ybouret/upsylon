
#include "y/lang/pattern.hpp"

namespace upsylon
{
    namespace Lang
    {

        ios::ostream & Pattern::emitAddress(ios::ostream &os) const
        {
            os.viz(this);
            return os;
        }
    }

}

