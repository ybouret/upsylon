
#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Terminal:: ~Terminal() throw() {}

            Terminal:: Terminal(const string &id, const Attribute flag ) :
            Rule(UUID,id),
            attr(flag)
            {
                data = &attr;
            }

            bool Terminal:: accept( Y_LANG_SYNTAX_RULE_ARGS ) const
            {
                if(verbose) { display(std::cerr, depth, '?') << std::endl; }
                Lexeme *lexeme = lexer.get(source);
                if(lexeme)
                {
                    if( *(lexeme->label) == name )
                    {
                        if(verbose) { display(std::cerr,depth,'+') << std::endl; }
                        Grow(tree,Node::Create(*this,lexeme));
                        //std::cerr << "..accepted " << name << std::endl;
                        return true;
                    }
                    else
                    {
                        if(verbose) { display(std::cerr,depth,'-') << std::endl; }
                        lexer.unget(lexeme);
                        return false;
                    }
                }
                else
                {
                    // EOF
                    if(verbose) { display(std::cerr,depth,'-') << "/EOF" << std::endl; }
                    return false;
                }
            }

            const char * Terminal:: __style() const throw()
            {
                switch(attr)
                {
                    case Univocal: return "rounded";
                    case Semantic: return "dotted";
                    default:
                        break;
                }
                return "bold";
            }
          
            bool Terminal:: hollow() const throw()
            {
                return false;
            }



        }

    }

}

