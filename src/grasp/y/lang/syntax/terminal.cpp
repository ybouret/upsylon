#include "y/lang/syntax/terminal.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Terminal:: ~Terminal() throw()
            {
            }

            Terminal:: Terminal(const string &n, const Attribute role) :
            Rule(UUID,n), attr(role)
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
                Y_LANG_SYNTAX_VERBOSE("|_" << typeName() << " '" << name << "'" << std::endl);
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
                        Y_LANG_SYNTAX_VERBOSE("|_accepted " << typeName() << " '" << name << "'" << std::endl);
                        return true;
                    }
                    else
                    {
                        Y_LANG_SYNTAX_VERBOSE("|_rejected " << typeName() << " '" << name << "'" << std::endl);
                        lexer.unget(next);
                        return false;
                    }
                }
            }

            const char * Terminal:: graphVizShape() const throw()
            {
                return "box";
            }

            void Terminal:: graphVizEpilog(ios::ostream &) const
            {

            }
        }
    }
}

