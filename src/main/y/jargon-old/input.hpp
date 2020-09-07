
#ifndef Y_JARGON_INPUT_INCLUDED
#define Y_JARGON_INPUT_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        typedef arc_ptr<ios::istream> Input; //!< alias to a shared input
        
        namespace Kernel {
            
            //! setup inputs for module
            struct MakeInput
            {
                static ios::istream * FromFile(const string &fileName);              //!< ios::icstream
                static ios::istream * FromData(const void *data, const size_t size); //!< ios::imstream, must be persistent
            };
        }
        
        
    }
    
}


#endif

