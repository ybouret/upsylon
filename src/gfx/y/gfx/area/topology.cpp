
#include "y/gfx/area/topology.hpp"


namespace upsylon
{
    namespace GFX
    {

        Topology:: ~Topology() throw()
        {
        }
        

        Topology:: Topology(const Area &area) :
        Object(),
        Area(area),
        Core(area),
        Boundaries(area)
        {
        }

        

        std::ostream & operator<<(std::ostream &os, const Topology &topo)
        {
            os << "topology.area         : "  << static_cast<const Area &>(topo) << std::endl;
            const Core       &core       = topo;
            os << core << std::endl;
            const Boundaries &boundaries = topo;
            os << boundaries;
            return os;
        }

    }


}

