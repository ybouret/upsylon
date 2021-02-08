
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
        core( hasCore() ? SubArea::CreatePointer(area,Position::Core) : NULL ),
        borders(*this),
        corners(*this)
        {

        }

        static inline void display(const AreaHandle &h, std::ostream &os)
        {
            if(h.is_empty()) os << "<emtpy>"; else os << *h;
        }

        std::ostream & operator<<(std::ostream &os, const Topology &topo)
        {
            os << "topology.area   : " << static_cast<const Area &>(topo) << std::endl;
            os << "topology.core   : "; display(topo.core,os); std::cerr << std::endl;

            os << "topology.bottom : "; display(topo.borders.bottom,os); std::cerr << std::endl;
            os << "topology.top    : "; display(topo.borders.top,os);    std::cerr << std::endl;
            os << "topology.left   : "; display(topo.borders.left,os);   std::cerr << std::endl;
            os << "topology.right  : "; display(topo.borders.right,os);  std::cerr << std::endl;


            os << "topology.bottom_left  : "; display(topo.corners.bottom_left,os); std::cerr << std::endl;
            os << "topology.bottom_right : "; display(topo.corners.bottom_right,os); std::cerr << std::endl;
            os << "topology.top_left     : "; display(topo.corners.top_left,os); std::cerr << std::endl;
            os << "topology.top_right    : "; display(topo.corners.top_right,os);  

            return os;
        }

    }


}

