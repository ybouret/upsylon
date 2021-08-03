
#include "y/alchemy/reactor.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        const char Reactor:: CLID[] = "Reactor";
        
        Reactor:: Reactor(Library    &_lib,
                          Equilibria &_eqs) :
        lib(_lib),
        eqs(_eqs),
        N(eqs->size()),
        M(lib->size()),
        lfrz(_lib,Library::CLID),
        efrz(_eqs,Equilibria::CLID)
        {
        }



    }

}
