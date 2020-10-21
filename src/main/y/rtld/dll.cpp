#include "y/rtld/dll.hpp"

namespace upsylon
{

    
    dll:: ~dll() throw()
    {
        rtld::quit(handle);
    }

 
    

}
