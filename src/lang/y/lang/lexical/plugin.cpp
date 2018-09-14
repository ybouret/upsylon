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

            Plugin:: Plugin(const string &id, const char *rx ) :
            Scanner(id),
            trigger(rx)
            {
            }

            Plugin:: Plugin(const char *id, const char *rx ) :
            Scanner(id),
            trigger(rx)
            {
            }

        }
    }
}

