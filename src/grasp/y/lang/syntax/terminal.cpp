#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Terminal:: ~Terminal() throw()
            {
            }

            Terminal:: Terminal(const string &n) :
            Rule(UUID,n)
            {
                derived = static_cast<Terminal *>(this);
            }

            const char * Terminal:: typeName() const throw()
            {
                return "Terminal";
            }

            bool Terminal:: isHollow() const throw()
            {
                return false;
            }

            Y_LANG_SYNTAX_ACCEPT_START(Terminal)
            {
                assert(NULL==tree||tree->internal);
                Lexeme *next = lexer.get(source);
                if(!next)
                {
                    return false;
                }
                else
                {
                    if( *(next->label) == name )
                    {
                        Node *leaf = Node::Create(*this,next);
                        Node::Grow(tree,leaf);
                        return true;
                    }
                    else
                    {
                        lexer.unget(next);
                        return false;
                    }
                }
            }
        }
    }
}

