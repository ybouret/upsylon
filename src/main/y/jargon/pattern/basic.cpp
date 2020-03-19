
#include "y/jargon/pattern/basic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Basic:: ~Basic() throw()
        {
            
        }
        
        Basic:: Basic(const uint32_t u) throw() : Pattern(u) {}
        
        bool Basic:: match(Token &token, Source &source) const
        {
            assert(token.size==0);
            Char *ch = source.get();
            if(!ch)
            {
                return false;
            }
            else
            {
                if( good(ch->code) )
                {
                    token.push_back(ch);
                    return true;
                }
                else
                {
                    source.unget(ch);
                    return false;
                }
            }
        }
    }
    
}
