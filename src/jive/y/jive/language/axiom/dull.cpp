
#include "y/jive/language/axiom/dull.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            Dull:: ~Dull() throw()
            {
            }

            Y_LANG_AXIOM_IMPL(Dull)
            {
                (void) lexer;
                (void) source;
                (void) tree;
                (void) obs;
                return false;
            }
        }
    }
}

