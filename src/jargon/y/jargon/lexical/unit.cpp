
#include "y/jargon/lexical/unit.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            Unit:: ~Unit() throw()
            {
            }
            
            Unit:: Unit(const Context &ctx,
                        const Tag     &t) throw() :
            Object(),
            Token(),
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
            
            
            void  Unit:: writeTo(exception &excp, const bool definite) const
            {
                if(definite)
                {
                    excp.cat("'");
                    cat(excp);
                    excp.cat("'");
                }
                else
                {
                    excp.cat("<%s>", **label);
                    if(size>0)
                    {
                        excp.cat(" = '");
                        cat(excp);
                        excp.cat("'");
                    }
                }
            }
            
        }
        
    }
    
}
