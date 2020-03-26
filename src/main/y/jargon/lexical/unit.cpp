
#include "y/jargon/lexical/unit.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
          
            Unit:: ~Unit() throw()
            {
            }
            
            Unit:: Unit(const Cache   &cache,
                        const Context &ctx,
                        const Tag     &t) throw() :
            Object(),
            Token(cache),
            Context(ctx),
            inode<Unit>(),
            label(t)
            {
                
            }
            
            
            void Unit:: append( const uint8_t code)
            {
                Token &self = *this;
                self.append(*this,code);
            }
            
            void Unit:: append( const string &code)
            {
                Token &self = *this;
                self.append(*this,code);
            }
            
            void Unit:: append(const Token &token)
            {
                Token &self = *this;
                for(const Char *ch=token.head;ch;ch=ch->next)
                {
                    self.append(*this,ch->code);
                }
            }
        }
        
    }
    
}
