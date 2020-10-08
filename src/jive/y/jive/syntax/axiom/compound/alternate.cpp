


#include "y/jive/syntax/axiom/compound/alternate.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Alternate:: ~Alternate() throw()
            {
            }

            Y_JIVE_AXIOM_ACCEPT_IMPL(Alternate)
            {
                Y_JIVE_PRINTLN("Alternate <" << name << ">");

                return false;
            }
        }
    }
}

