#include "y/jargon/lexical/plugin/error.hpp"
#include "y/jargon/lexical/analyzer.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            Error:: ~Error() throw()
            {
                
            }
            
            
            void Error:: setup()
            {
                back("[:endl:]",     this, & Error:: onEndl);
                back("[:blank:]",    this, & Error:: onEmit);
                discard("any1", ".", this, & Error:: onChar);
            }
            
            void Error:: onInit(const Token &token)
            {
                assert(token.size>0);
                const Context & context = *(token.head);
                unit = new Unit(context,label);
                unit->append(token);
            }
            
            void Error:: onEndl(const Token &_)
            {
                newLine(_);
                onEmit(_);
            }
            
            void Error:: onEmit(const Token &)
            {
                assert(unit.is_valid());
                lexer.unget( unit.yield() );
            }
            
            void Error:: onChar(const Token &token)
            {
                assert(unit.is_valid());
                unit->append(token);
            }
        }
    }
}


