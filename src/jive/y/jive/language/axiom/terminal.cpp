
#include "y/jive/language/axiom/terminal.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            Terminal:: ~Terminal() throw()
            {
            }

            bool Node:: isTerminal() const throw()
            {
                return axiom.uuid == Terminal::UUID;
            }

            bool Node:: isInternal() const throw()
            {
                return axiom.uuid != Terminal::UUID;
            }
        }

    }
}

