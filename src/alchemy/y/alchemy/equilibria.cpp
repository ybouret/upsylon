
#include "y/alchemy/equilibria.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        Equilibria::const_type & Equilibria:: bulk() const throw()
        {
            return edb;
        }

        Equilibria:: ~Equilibria() throw()
        {
            
        }
        
        Equilibria:: Equilibria() : edb(), enw(), tdisp(0)
        {
        }

        const char Equilibria::CLID[] = "Equilibria";

        Equilibrium & Equilibria:: operator()(Equilibrium *eq)
        {
            assert(eq);
            const Equilibrium::Pointer p(eq);
            if(!edb.insert(p)) throw exception("%s: multipliple equilibrium '%s'", CLID, *(eq->name) );
            aliasing::_(enw) = max_of(enw,eq->name.size());
            return *eq;
        }
        
        
    }
}

