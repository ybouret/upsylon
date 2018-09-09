
#include "y/lang/pattern.hpp"

namespace upsylon
{
    namespace Lang
    {

        Pattern:: ~Pattern() throw()
        {
        }

        ios::ostream & Pattern::emitAddress(ios::ostream &os) const
        {
            os.viz(this);
            return os;
        }

        Pattern:: Pattern(const uint32_t t) throw() : uuid(t) {}
        
    }

}

