
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
        core(0)
        {

        }

    }


}

