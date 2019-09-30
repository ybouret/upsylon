
#include "y/geometry/iso3d/points.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            Point_:: ~Point_() throw()
            {
            }

            Point_:: Point_( const Location &l, const Vertex &v) throw() :
            location(l),
            position(v)
            {
            }

            const Location & Point_ :: key() const throw()
            {
                return location;
            }
            

        }

    }
}
