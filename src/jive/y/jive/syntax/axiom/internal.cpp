
#include "y/jive/syntax/axiom/internal.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Internal:: ~Internal() throw()
            {
            }

            Dull:: ~Dull() throw()
            {
            }
            
            Y_JIVE_AXIOM_ACCEPT_IMPL(Dull)
            {
                (void)tree;
                (void)lexer;
                (void)source;
                return false;
            }
        }

    }

}

