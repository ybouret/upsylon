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

            Plugin:: Plugin(Translator &t, const string &id, const char *rx ) :
            Scanner(id),
            trigger(rx),
            lexer(t)
            {
            }

            Plugin:: Plugin(Translator &t,const string &id, const string &rx ) :
            Scanner(id),
            trigger(rx),
            lexer(t)
            {
            }

        }
    }
}

