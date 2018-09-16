
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

            Rule & Parser:: __term( const string &id, const string &rx, const Terminal::Attribute attr)
            {
                // register in lexer
                Lexer   &lexer = *this;
                Scanner &root  = *lexer;

                // register in grammar
                root.emit(id,rx);
                return terminal(id,attr);
            }


        }
    }
}

