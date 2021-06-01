#include "y/jive/language/axiom.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{

    namespace Jive
    {
        namespace Language
        {
            string   Axiom:: gvName() const
            {
                return string_convert::to_printable(*name);
            }

            Axiom:: ~Axiom() throw()
            {
            }

        }
    }

}


