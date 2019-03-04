#include "y/lang/syntax/terminal.hpp"
#include "y/string/convert.hpp"
#include "y/exception.hpp"

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
            Rule(UUID,n), attr( Dangling )
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

            bool Terminal:: isDangling() const throw() { return 0==attr; }

            Terminal & Terminal:: setStandard()
            {
                if(!isDangling())
                {
                    throw exception("Terminal.setStandard(<%s> was already set)", *name);
                }
                (attr_t &)attr |=  Standard;
                return *this;
            }

            Terminal & Terminal:: setUnivocal()
            {
                if(!isDangling())
                {
                    throw exception("Terminal.setUnivocal(<%s> was already set)", *name);
                }
                (attr_t &)attr |=  Univocal;
                return *this;
            }

            Terminal & Terminal:: setOperator()
            {
                if(isSemantic())
                {
                    throw exception("Terminal.setOperator(<%s> declared as semantic )", *name);
                }
                (attr_t&)attr |= Operator;
                return *this;
            }

            Terminal & Terminal:: setSemantic()
            {
                if(isOperator())
                {
                    throw exception("Terminal.setSemantic(<%s> declared as operator)", *name);
                }
                (attr_t&)attr |= Semantic;
                return *this;
            }





            Y_LANG_SYNTAX_ACCEPT_START(Terminal)
            {
                assert(NULL==tree||tree->internal);
                Y_LANG_SYNTAX_VERBOSE("|_" << typeName() << " <" << name << ">" << std::endl);
                Lexeme *next = lexer.get(source);
                if(!next)
                {
                    Y_LANG_SYNTAX_VERBOSE( "|_EOF" << std::endl );
                    return false;
                }
                else
                {
                    if( *(next->label) == name )
                    {
                        Node *leaf = Node::Create(*this,next);
                        Node::Grow(tree,leaf);
                        Y_LANG_SYNTAX_VERBOSE("|_accepted " << typeName() << " <" << name << ">" << std::endl);
                        return true;
                    }
                    else
                    {
                        Y_LANG_SYNTAX_VERBOSE("|_rejected " << typeName() << " <" << name << ">" << std::endl);
                        lexer.unget(next);
                        return false;
                    }
                }
            }

            const char * Terminal:: graphVizShape() const throw()
            {
                if(isOperator())
                {
                    return "triangle";
                }
                else
                {
                    return "box";
                }
            }

            void Terminal:: graphVizEpilog(ios::ostream &) const
            {
                
            }

            const char * Terminal:: graphVizStyle() const throw()
            {
                switch(attr)
                {
                    case Dangling: return "diagonals";
                    case Standard: return "bold,filled";
                    case Standard|Operator: return "bold,filled";
                    case Standard|Semantic: return "bold,dashed,filled";

                    case Univocal:          return "bold,rounded";
                    case Univocal|Operator: return "bold,rounded";
                    case Univocal|Semantic: return "bold,dashed,rounded";
                        
                    default:
                        break;
                }
                return "bold";
            }
        }
    }
}

