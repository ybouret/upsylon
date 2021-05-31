#include "y/jive/language/axiom.hpp"

namespace upsylon
{

    namespace Jive
    {
        namespace Language
        {
            string   Axiom:: gvName() const
            {
                return name->to_printable();
            }

            Axiom:: ~Axiom() throw()
            {
            }

        }
    }

}


