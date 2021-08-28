

#include "y/chemical/species.hpp"
 
namespace upsylon
{
    namespace Chemical
    {
        Species:: ~Species() throw()
        {
            aliasing::_(charge) = 0;
            aliasing::_(rating) = 0;
        }
        
        const string & Species:: key() const throw() { return name; }
    }
    
}

