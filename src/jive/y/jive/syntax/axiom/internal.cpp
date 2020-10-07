
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
                (void)depth;
                return false;
            }

            void Dull:: attach(Axiom::Registry &db) const
            {
                (void) new_in(db);
            }
            
        }

    }

}

