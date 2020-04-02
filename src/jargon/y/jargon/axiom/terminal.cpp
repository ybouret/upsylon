
#include "y/jargon/axiom/terminal.hpp"
#include "y/jargon/xcache.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {

        Terminal:: ~Terminal() throw()
        {
        }
        
        void Terminal:: setup()
        {
            self = static_cast<Terminal *>(this);
            Y_JAXIOM(std::cerr << "+[Terminal] <" << label << ">" << std::endl);
            const string id = **label;
            if(id.size()>0)
            {
                switch(id[0])
                {
                    case '+':
                    case '?':
                    case '*':
                    case '#':
                        throw exception("Jargon::Terminal(invalid label <%s>)",*id);
                        
                    default:
                        break;
                }
            }
        }

        
        bool Terminal:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            assert(NULL==xtree||xtree->isInternal());
            Lexeme *lexeme = lexer.get(source);
            if( lexeme )
            {
                if( *(lexeme->label) == *label )
                {
                    Y_JAXIOM(std::cerr << "[Terminal]<" << label << "> = '" << *lexeme << "'" << std::endl);
                    XNode::Advance(xtree,xcache.query(*this, lexeme));
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
                // EOS
                return false;
            }
        }

        ios::ostream & Terminal:: display(ios::ostream &fp) const
        {
            return fp("|_[Terminal<%s>]\n", **label);
        }

    }
    
}


