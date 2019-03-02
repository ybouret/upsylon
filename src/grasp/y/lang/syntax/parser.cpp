
#include "y/lang/syntax/parser.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Parser:: ~Parser() throw()
            {
            }

            Parser:: Parser(const string &id) :
            Lexer(id), Grammar(label), CountedObject()
            {
            }

            Parser:: Parser(const char *id) :
            Lexer(id), Grammar(label), CountedObject()
            {
            }

            const string & Parser:: key() const throw()
            {
                return *name;
            }

        }
    }
}
