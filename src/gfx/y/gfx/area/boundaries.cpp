#include "y/gfx/area/boundaries.hpp"


namespace upsylon
{
    namespace GFX
    {

        Boundaries:: Boundaries(const Area &area) :
        Borders(area),
        Corners(area)
        {}

        Boundaries:: ~Boundaries() throw()
        {
        }

        unit_t Boundaries:: items() const throw()
        {
            return Borders::items() + Corners::items();
        }

        std::ostream & operator<<(std::ostream &os, const Boundaries &boundaries)
        {
            const Borders &borders = boundaries;
            const Corners &corners = boundaries;
            os << borders << std::endl << corners;
            return os;
        }
    }
}
