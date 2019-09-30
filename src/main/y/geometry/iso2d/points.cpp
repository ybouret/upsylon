
#include "y/geometry/iso2d/points.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            Point_:: ~Point_() throw()
            {
            }

            const Edge & Point_:: key() const throw()
            {
                return location;
            }

            Point_:: Point_( const Edge &l, const Vertex &v) throw() :
            location(l),
            position(v)
            {
            }

            Node:: Node( const Point &p ) throw() :
            Point(p), next(0), prev(0)
            {
            }

            Node:: ~Node() throw()
            {
            }
            

        }
    }
}

