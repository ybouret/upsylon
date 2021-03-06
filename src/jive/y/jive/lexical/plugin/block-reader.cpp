
#include "y/jive/lexical/plugin/block-reader.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {
         
            BlockReader :: ~BlockReader() throw()
            {
            }
            
            void BlockReader:: setup()
            {
                discard("endl","[:endl:]", this, &BlockReader::OnEndl);
                discard("char","[:dot:]",  this, &BlockReader::OnChar);
            }
            
            void BlockReader:: OnInit(const Token &t)
            {
                assert(t.size>0);
                block = NULL;
                block = Unit::Create(*(t.head),label,stamp);
            }
            
            void BlockReader:: OnQuit(const Token &)
            {
                assert(block.is_valid());
                Q.push(block.yield());
            }
            
            void BlockReader:: OnEndl(const Token &t)
            {
                assert(block.is_valid());
                assert(t.size>0);
                origin->newLine();
                block->push( Char::Copyset(t.head,'\n') );
            }

            void BlockReader:: OnChar(const Token &t)
            {
                assert(block.is_valid());
                assert(1==t.size);
                block->push( Char::Copycat(t.head) );
            }
            
            
        }
    }
}
