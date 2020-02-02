#include "y/lang/syntax/rule/terminal.hpp"
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
            Rule(UUID,n, typeid(Terminal) ),
            ordinary(true),
            univocal(false),
            attr(Standard)
            {
                derived = static_cast<Terminal *>(this);
            }


            Terminal & Terminal:: sm()
            {
                if(Standard!=attr)
                {
                    throw exception("%s <%s>.sm() is NOT Standard", typeName(), *name);
                }
                (Attribute &)attr = Semantic;
                return *this;
            }

            Terminal & Terminal:: op()
            {
                if(Standard!=attr)
                {
                    throw exception("%s <%s>.op() is NOT Standard", typeName(), *name);
                }
                (Attribute &)attr = Operator;
                return *this;
            }


            const char * Terminal:: typeName() const throw()
            {
                return "Terminal  ";
            }

            bool Terminal:: isHollow() const throw()
            {
                return false;
            }

            void Terminal:: checkReady() const
            {
                // do nothing
            }

            Y_LANG_SYNTAX_ACCEPT_START(Terminal)
            {
                assert(NULL==tree||tree->internal);
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_" << typeName() << " <" << name << ">" << std::endl);
                Lexeme *next = lexer.get(source);
                
                if(!next)
                {
                    Y_LANG_SYNTAX_VERBOSE(std::cerr << "|_EOF" << std::endl );
                    return false;
                }
                else
                {
                    if( *(next->label) == name )
                    {
                        Node *leaf = Node::Create(*this,next);
                        Node::Grow(tree,leaf);
                        Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_accepted " << typeName() << " <" << name << ">" << std::endl);
                        return true;
                    }
                    else
                    {
                        Y_LANG_SYNTAX_VERBOSE(std::cerr <<"|_rejected " << typeName() << " <" << name << ">" << std::endl);
                        lexer.unget(next);
                        return false;
                    }
                }
            }

            const char * Terminal:: graphVizShape() const throw()
            {
                if(Operator==attr)
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

                if(ordinary)
                {
                    assert(!univocal);
                    switch (attr)
                    {
                        case Standard: return "bold,filled";
                        case Operator: return "bold,filled";
                        case Semantic: return "bold,dashed";
                    }
                }
                else
                {
                    assert(univocal);
                    switch (attr)
                    {
                        case Standard: return "bold,filled,rounded";
                        case Operator: return "bold,filled,rounded";
                        case Semantic: return "bold,dashed,rounded";
                    }
                }
                return "bold";
            }

            
            
        }
    }
}

