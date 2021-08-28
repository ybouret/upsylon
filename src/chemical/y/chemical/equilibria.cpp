
#include "y/chemical/equilibria.hpp"

namespace upsylon
{
    namespace Chemical
    {
        const char Equilibria::CLID[] = "Chemical::Equilibria";
        
        Equilibria:: ~Equilibria() throw()
        {
        }
        
        Equilibria:: Equilibria() : edb()
        {
        }
        
        Equilibria::const_type &Equilibria:: bulk() const throw()
        {
            return edb;
        }
            

    }
}
