#include "y/lang/lexical/plugin.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Plugin:: ~Plugin() throw()
            {
            }

            Plugin:: Plugin(const string &id) : Scanner(id)
            {
            }
            
        }
    }
}

