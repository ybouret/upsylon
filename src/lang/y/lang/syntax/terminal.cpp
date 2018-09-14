
#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Terminal:: ~Terminal() throw() {}

            Terminal:: Terminal(const string &id ) : Rule(UUID,id)
            {
            }

            bool Terminal:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                Lexeme *lexeme = lexer.get(source);
                if(lexeme)
                {
                    if( *(lexeme->label) == name )
                    {
                        Grow(tree,Node::Create(*this,lexeme));
                        return true;
                    }
                    else
                    {
                        lexer.unget(lexeme);
                        return false;
                    }
                }
                else
                {
                    // EOF
                    return false;
                }
            }

          

        }

    }

}

