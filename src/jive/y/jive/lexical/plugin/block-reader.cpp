
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
            
            void BlockReader:: OnInit(const Token &)
            {
                block.release();
            }
            
            void BlockReader:: OnQuit(const Token &)
            {
                
            }
            
            void BlockReader:: OnEndl(const Token &t)
            {
                assert(t.size>0);
                origin->newLine();
                block << Char::Copyset(t.head_char(),'\n');
            }
            
            
        }
    }
}
