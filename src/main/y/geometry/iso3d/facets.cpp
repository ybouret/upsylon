#include "y/geometry/iso3d/facets.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            Facet:: ~Facet() throw()
            {
            }

            
            Facet:: Facet(const Point &A, const Point &B, const Point &C) throw() :
            a(A),
            b(B),
            c(C) //,e3( A->location, B->location, C->location )
            {
            }

        }

    }
}


