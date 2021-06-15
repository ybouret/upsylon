
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
                    case Standard: return "Standard";
                    case Univocal: return "Univocal";
                    case Division: return "Division";
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
                Y_LANG_PRINTLN( obs.indent() << typeText() << " <" << name << ">");
                Lexeme *lx = lexer.get(source);
                if(!lx)
                {
                    Y_LANG_PRINTLN(  obs.indent() << "[" << Rejected << " EOS]" );
                    return false;
                }
                else
                {
                    if(lx->label != name)
                    {
                        Y_LANG_PRINTLN(  obs.indent() << "[" << Rejected << " <" << lx->label << "> = '" << *lx << "']" );
                        lexer.unget(lx);
                        return false;
                    }
                    else
                    {
                        Node::Grow(tree, Node::Acquire(*this,lx) );
                        Y_LANG_PRINTLN( obs.indent() << "[" << Accepted << " <" << name << "> = '" << *lx << "']" );
                        return true;
                    }

                }
            }

            
            const Lexeme * Node:: LastLexeme(const Node *node) throw()
            {
                assert(node);
                switch(node->state)
                {
                    case IsTerminal: return node->lexeme();
                    case IsInternal: break;
                }
                
                const List &self = node->leaves();
                return self.size ? LastLexeme(self.tail) : NULL;
            }
        }

    }
}

