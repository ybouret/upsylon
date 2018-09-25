
#include "y/chem/species.hpp"


namespace upsylon
{
    namespace Chemical
    {
        Object :: Object() throw()
        {
        }

        Object:: ~Object() throw()
        {
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            os << '(' << sp.name << ',' << sp.z << ')';
            return os;
        }
    }

}

