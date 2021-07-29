

#include "y/alchemy/species.hpp"

namespace upsylon
{
    namespace alchemy
    {
        
        species:: ~species() throw()
        {
        }
        
        species:: species(const species &other) :
        name(other.name),
        z(other.z)
        {
        }

        const string & species:: key() const throw()
        {
            return name;
        }


    }
    
}
