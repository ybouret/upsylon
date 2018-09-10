
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/joker.hpp"

namespace upsylon
{
    namespace Lang
    {

        Pattern * Pattern::Optimize( Pattern *p ) throw()
        {
            assert(p);

            switch(p->uuid)
            {
                default:
                    break;
            }
            return p;
        }
    }

}

