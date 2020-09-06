
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
                //block.release();
            }
            
            void BlockReader:: OnEndl(const Token &)
            {
                origin->newLine();
                
            }
            
            
        }
    }
}
