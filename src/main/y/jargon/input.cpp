#include "y/jargon/input.hpp"

#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Kernel  {
         
            ios::istream * MakeInput::FromFile(const string &fileName)
            {
                return new ios::icstream(fileName);
            }
            
            ios::istream * MakeInput::FromData(const void *data, const size_t size)
            {
                return new ios::imstream(data,size);
            }
            
            
        }
        
    }
    
}
