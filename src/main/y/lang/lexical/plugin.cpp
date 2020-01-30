#include "y/lang/lexical/lexer.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Plugin:: ~Plugin() throw()
            {
            }

            Plugin:: Plugin(Translator &t, const string &id, const char *rx) :
            Scanner(id),
            trigger(rx),
            lexer(t)
            {
                verbose=(*t).verbose;
            }

            Plugin:: Plugin(Translator &t,const string &id, const string &rx) :
            Scanner(id),
            trigger(rx),
            lexer(t)
            {
                verbose=(*t).verbose;
            }

            const Translator * Plugin:: attached() const throw()
            {
                return &lexer;
            }
            

        }
    }
}

