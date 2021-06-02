
#include "y/jive/language/axiom/terminal.hpp"
#include "y/ios/indent.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            Terminal:: ~Terminal() throw()
            {
            }

            Y_LANG_AXIOM_IMPL(Terminal)
            {
                Y_LANG_PRINTLN( obs.indent() << "<" << name << ">");
                Lexeme *lx = lexer.get(source);
                if(!lx)
                {
                    Y_LANG_PRINTLN(  obs.indent() << "=> [reject EOS]" );
                    return false;
                }
                else
                {
                    if(lx->label != name)
                    {
                        Y_LANG_PRINTLN(  obs.indent() << "=> [reject " << lx->label << "]" );
                        lexer.unget(lx);
                        return false;
                    }
                    else
                    {
                        Node::Grow(tree, Node::Acquire(*this,lx) );
                        Y_LANG_PRINTLN(  obs.indent() << "=> [accept]" );
                        return true;
                    }

                }
            }

        }

    }
}

