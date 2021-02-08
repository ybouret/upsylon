
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
        Area(area),
        core( hasCore() ? new SubArea(area,Position::Core) : NULL ),
        inside( core.is_valid() ? core->n : 0 ),
        borders(*this),
        corners(*this),
        outside(borders.items+corners.items)
        {
        }

        

        std::ostream & operator<<(std::ostream &os, const Topology &topo)
        {
            os << "topology.area         : "  << static_cast<const Area &>(topo) << std::endl;
            os << "topology.inside       : #" << topo.inside << std::endl;
            os << "topology.core         : "  << topo.core   << std::endl;
            os << "topology.outside      : #" << topo.outside << std::endl;
            os << "topology.borders      : #" << topo.borders.items << std::endl;
            os << "topology.bottom       : "  << topo.borders.bottom << std::endl;
            os << "topology.top          : "  << topo.borders.top    << std::endl;
            os << "topology.left         : "  << topo.borders.left   << std::endl;
            os << "topology.right        : "  << topo.borders.right  << std::endl;

            os << "topology:corners      : #" << topo.corners.items        << std::endl;
            os << "topology.bottom_left  : "  << topo.corners.bottom_left  << std::endl;
            os << "topology.bottom_right : "  << topo.corners.bottom_right << std::endl;
            os << "topology.top_left     : "  << topo.corners.top_left     << std::endl;
            os << "topology.top_right    : "  << topo.corners.top_right;

            return os;
        }

    }


}

