
#include "y/aqua/equilibrium/component.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace Aqua
    {
        Component:: ~Component() throw()
        {
        }

        Component::Component(const Species &sp_, const int nu_) throw() :
        Object(),
        dnode<Component>(),
        sp(sp_),
        nu(nu_),
        p( abs_of(nu) ),
        pm1( p-1 )
        {
            //std::cerr << "+component " << sp.name << " : nu=" << nu << std::endl;
        }

        
        
    }

}

