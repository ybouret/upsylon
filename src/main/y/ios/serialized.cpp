#include "y/ios/serialized.hpp"

namespace upsylon {

    namespace ios {

        
        digest  serialized::md(const serializable &input, hashing::function &H )
        {
            H.set();
            input.hash_with(H);
            return H.md();
        }
        
    }

}
