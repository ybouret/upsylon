
#include "y/geometry/iso3d/points.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            Point_:: ~Point_() throw()
            {
            }

            Point_:: Point_( const Edge &l, const Vertex &v) throw() :
            location(l),
            position(v)
            {
            }

            

        }

    }
}
