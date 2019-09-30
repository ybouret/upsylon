
//! \file
#ifndef Y_GEOMETRY_ISO3D_POINTS_INCLUDED
#define Y_GEOMETRY_ISO3D_POINTS_INCLUDED 1

#include "y/geometry/iso3d/location.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/set.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {
            
            class Point_ : public counted_object
            {
            public:
                const Location location;
                const Vertex   position;

                explicit Point_( const Location &, const Vertex &) throw();
                virtual ~Point_() throw();

                const  Location & key() const throw();

                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point_);
            };

            typedef intr_ptr<Location,Point_> Point;

            typedef set<Location,Point,Location::Hasher,memory::global> PointsType;


            class Points : public PointsType
            {
            public:
                explicit Points() throw();
                virtual ~Points() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Points);
            };


        }

    }

}

#endif

