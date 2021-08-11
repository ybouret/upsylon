#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"


namespace upsylon
{

    namespace Alchemy
    {

        Equilibrium & Equilibria:: water(Library       &lib,
                                         const double   Kw)
        {
            Equilibrium &eq = (*this)("water",Kw);
            eq(1, lib.get("H+") );
            eq(1, lib.get("HO-"));
            return eq;
        }


    }

}

