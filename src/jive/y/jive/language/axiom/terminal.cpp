
#include "y/jive/language/axiom/terminal.hpp"
#include "y/ios/indent.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            const char * Terminal:: TypeText(const Type t) throw()
            {
                switch(t)
                {
                    case Standard: return "standard";
                    case Univocal: return "univocal";
                    case Division: return "division";
                }
                return "???";
            }

            const char * Terminal:: typeText() const throw()
            {
                return TypeText(type);
            }


            Terminal:: ~Terminal() throw()
            {
            }

            Y_LANG_AXIOM_IMPL(Terminal)
            {
                Lexeme *lx = lexer.get(source);
                if(!lx)
                {
                    return false;
                }
                else
                {
                    if(lx->label != name)
                    {
                        lexer.unget(lx);
                        return false;
                    }
                    else
                    {
                        Node::Grow(tree, Node::Acquire(*this,lx) );
                        return true;
                    }

                }
            }

            
            const Lexeme * Node:: LastLexeme(const Node *   node,
                                             const Axiom * &from) throw()
            {
                assert(node);
                switch(node->state)
                {
                    case IsTerminal: return node->lexeme();
                    case IsInternal: break;
                }
                
                const List &self = node->leaves();
                from             = & node->axiom;
                return self.size ? LastLexeme(self.tail,from) : NULL;
            }
        }

    }
}

