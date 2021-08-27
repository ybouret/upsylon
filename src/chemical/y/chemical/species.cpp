

#include "y/chemical/species.hpp"
 
namespace upsylon
{
    namespace Chemical
    {
        Species:: ~Species() throw()
        {
            aliasing::_(z) = 0;
        }
    }
    
}

