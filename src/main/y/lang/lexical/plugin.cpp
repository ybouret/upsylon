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

            Plugin:: Plugin(Translator &t, const string &id, const char *rx, const bool isVerbose) :
            Scanner(id),
            trigger(rx),
            lexer(t)
            {
                verbose = isVerbose;
            }

            Plugin:: Plugin(Translator &t,const string &id, const string &rx,const bool isVerbose) :
            Scanner(id),
            trigger(rx),
            lexer(t)
            {
                verbose=isVerbose;
            }

            

        }
    }
}

