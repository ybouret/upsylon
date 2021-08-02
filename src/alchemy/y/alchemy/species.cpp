#include "y/alchemy/species.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        
        Species:: ~Species() throw()
        {
        }
        
        

        const string & Species:: key() const throw()
        {
            return name;
        }


    }
    
}
