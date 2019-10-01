
//! \file
#ifndef Y_GEOMETRY_ISO3D_FACETS_INCLUDED
#define Y_GEOMETRY_ISO3D_FACETS_INCLUDED 1

#include "y/geometry/iso3d/points.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            //! a 3D facet
            class Facet : public counted_object
            {
            public:
                const Point a;  //!< computed point
                const Point b;  //!< computed point, a!=b
                const Point c;  //!< computed point, a!=b && a!=c
                //const Edge3 e3; //!< signature from edges

                //! setup
                explicit Facet(const Point &A, const Point &B, const Point &C) throw();

                //! cleanup
                virtual ~Facet() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Facet);
            };

        }
    }
}
#endif

