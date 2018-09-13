
#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Terminal:: ~Terminal() throw() {}

            Terminal:: Terminal(const string &id ) : Rule(UUID), label(id)
            {
            }
            
        }

    }

}

