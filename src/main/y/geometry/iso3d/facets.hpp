
//! \file
#ifndef Y_GEOMETRY_ISO3D_FACETS_INCLUDED
#define Y_GEOMETRY_ISO3D_FACETS_INCLUDED 1

#include "y/geometry/iso3d/points.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            class Facet : public counted_object
            {
            public:
                const Point a;
                const Point b;
                const Point c;

                explicit Facet(const Point &A, const Point &B, const Point &C) throw();
                virtual ~Facet() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Facet);
            };

        }
    }
}
#endif

