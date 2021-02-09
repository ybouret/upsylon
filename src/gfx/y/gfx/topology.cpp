
#include "y/gfx/topology.hpp"


namespace upsylon
{
    namespace GFX
    {

        Topology:: ~Topology() throw()
        {
        }
        

        Topology:: Topology(const Area &area) :
        Object(),
        Boundaries(area),
        Area(area),
        core( hasCore() ? new SubArea(area,Position::Core) : NULL ),
        inner( core.is_valid() ? core->n : 0 ),
        outer( Boundaries::items() )
        {
        }

        

        std::ostream & operator<<(std::ostream &os, const Topology &topo)
        {
            os << "topology.area         : "  << static_cast<const Area &>(topo) << std::endl;
            os << "topology.inside       : #" << topo.inner << std::endl;
            os << "topology.core         : "  << topo.core   << std::endl;

            os << "topology.outside      : #" << topo.outer << std::endl;
            const Boundaries &boundaries = topo;
            os << boundaries;
            

            return os;
        }

    }


}

