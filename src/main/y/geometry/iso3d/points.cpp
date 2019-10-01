
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

            const Edge & Point_ :: key() const throw()
            {
                return location;
            }
            

        }

    }
}



namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            Points:: Points() throw() : PointsType() {}

            Points:: ~Points() throw()
            {
            }

        }

    }

}


