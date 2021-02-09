#include "y/gfx/area/boundaries.hpp"


namespace upsylon
{
    namespace GFX
    {

        Boundaries:: Boundaries(const Area &area) :
        Borders(area),
        Corners(area),
        outer( on_vertex + on_edge )
        {}

        Boundaries:: ~Boundaries() throw()
        {
        }
        

        std::ostream & operator<<(std::ostream &os, const Boundaries &boundaries)
        {
            const Borders &borders = boundaries;
            const Corners &corners = boundaries;
            os << "<boundaries.outer     : #" << boundaries.outer << ">" << std::endl;
            os << borders << std::endl;
            os << corners << std::endl;
            os << "<boundaries>";
            return os;
        }
    }
}
