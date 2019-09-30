
//! \file
#ifndef Y_GEOMETRY_ISO3D_POINTS_INCLUDED
#define Y_GEOMETRY_ISO3D_POINTS_INCLUDED 1

#include "y/geometry/iso3d/edge.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {
            
            class Point_ : public counted_object
            {
            public:
                const Edge   location;
                const Vertex position;
                const Edge & key() const throw();

                explicit Point_( const Edge &, const Vertex &) throw();
                virtual ~Point_() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point_);
            };

            typedef intr_ptr<Edge,Point_> Point;

            typedef set<Edge,Point,Edge::Hasher,memory::global> PointsType;


            class Points : public PointsType
            {
            public:

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Points);
            };


        }

    }

}

#endif

