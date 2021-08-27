

#include "y/chemical/labeled.hpp"
 
namespace upsylon
{
    namespace Chemical
    {

        Labeled:: ~Labeled() throw()
        {
            aliasing::_(nmax) = 0;
        }
    }
    
}

