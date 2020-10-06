
#include "y/jive/syntax/axiom/terminal.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Terminal:: ~Terminal() throw()
            {
            }

            Y_JIVE_AXIOM_ACCEPT_IMPL(Terminal)
            {
                Y_JIVE_PRINTLN("<"<<name<<">");
                Lexeme *lexeme = lexer.get(source);
                if(lexeme)
                {
                    Lexeme::Pointer lx(lexeme);
                    if( *(lexeme->label) == *name )
                    {
                        Y_JIVE_PRINTLN("|_accept '"<<*lexeme<<"'");
                        Node *leaf = Node::Acquire(*this,lx.yield());
                        Grow(tree,leaf);
                        return true;
                    }
                    else
                    {
                        Y_JIVE_PRINTLN("|_reject " << lexeme->label);
                        lexer.unget(lx.yield());
                        return false;
                    }
                }
                else
                {
                    Y_JIVE_PRINTLN("|_no lexeme");
                    return false;
                }

            }
            
        }

    }

}

