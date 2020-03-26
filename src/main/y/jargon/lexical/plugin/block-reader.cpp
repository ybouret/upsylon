
#include "y/jargon/lexical/plugin/block-reader.hpp"
#include "y/jargon/lexical/analyzer.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            BlockReader:: ~BlockReader() throw()
            {
                
            }
            
            
            void BlockReader:: setup()
            {
                endl("endl", "[:endl:]");
                discard("dot", ".", this, & BlockReader::onChar);
            }
            
            void BlockReader:: onInit(const Token &token)
            {
                assert(token.size>0);
                const Context & context = *(token.head);
                const Cache   & cache   = token.cache;
                unit = new Unit(cache,context,label);
            }
            
            
            void BlockReader:: onEmit(const Token &token)
            {
                assert(token.size>0);
                assert(unit.is_valid());
                
                lexer.unget( unit.yield() );
            }
            
            void BlockReader:: onChar(const Token &token)
            {
                assert(unit.is_valid());
                unit->append(token);
            }
        }
    }
}


