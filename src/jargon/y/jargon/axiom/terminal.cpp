
#include "y/jargon/axiom/terminal.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {

        Terminal:: ~Terminal() throw()
        {
        }
        
        
        bool Terminal:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            assert(NULL==xtree||xtree->isInternal());
            Lexeme *lexeme = lexer.get(source);
            if( lexeme )
            {
                if( *(lexeme->label) == *label )
                {
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


    }
    
}


