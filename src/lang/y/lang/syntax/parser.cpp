
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
            Lexical::Translator(id),
            Grammar(label)
            {
            }

        }
    }
}

