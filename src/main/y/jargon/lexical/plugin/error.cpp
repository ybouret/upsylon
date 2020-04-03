
#include "y/jargon/lexical/plugin/error.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            Error:: ~Error() throw()
            {
            }
            
            
            void Error::setup()
            {
                discard("chars", "[:word:]+",     this, &Error::onGrow);
                discard("other", "[\\x00-\\xff]", this, &Error::onDone);
            }
        
            void Error:: onInit(const Token &token)
            {
                assert(token.size>0);
                const Context & context = *(token.head);
                const Cache   & cache   = token.cache;
                bad = new Unit(cache,context,label);
                bad->append(token);
            }
            
            void Error:: onGrow(const Token &token)
            {
                assert(bad.is_valid());
                bad->append(token);
            }
            
            void Error:: onDone(const Token &)
            {
                assert(bad.is_valid());
                const Context &ctx = *(bad->head);
                exception excp("%s:%d:%d: syntax error with '",
                               **(ctx.tag), ctx.line, ctx.column);
                bad->cat(excp);
            }
            
        }
        
        
        
    }
}

