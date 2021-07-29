

#include "y/alchemy/species.hpp"

namespace upsylon
{
    namespace alchemy
    {
        
        species:: ~species() throw()
        {
        }
        
        species:: species(const species &other) :
        object(), counted(),
        name(other.name),
        z(other.z),
        indx(other.indx)
        {
        }

        const string & species:: key() const throw()
        {
            return name;
        }


    }
    
}
