
#include "y/lang/parser.hpp"

namespace upsylon
{
    namespace Lang
    {

        Parser:: ~Parser() throw()
        {
        }

        Parser:: Parser(const string &id) :
        Lexical::Translator(id),
        Syntax::Grammar(label)
        {
        }
        

    }
}
