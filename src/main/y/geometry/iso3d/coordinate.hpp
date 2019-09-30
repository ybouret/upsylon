//! \file
#ifndef Y_GEOMETRY_ISO3D_COORD_INCLUDED
#define Y_GEOMETRY_ISO3D_COORD_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/hashing/function.hpp"

namespace upsylon {

    namespace geometry {
        
        namespace Iso3D {

            typedef point3d<double> Vertex;

            class Coordinate
            {
            public:
                const unit_t i;
                const unit_t j;
                const unit_t k;

                Coordinate(const unit_t,const unit_t,const unit_t) throw();
                ~Coordinate() throw();
                Coordinate(const Coordinate &) throw();

                static int Compare(const Coordinate &,const Coordinate &) throw();

                friend bool operator==(const Coordinate &, const Coordinate &) throw();
                friend bool operator!=(const Coordinate &, const Coordinate &) throw();
                friend bool operator<(const Coordinate &,const Coordinate &) throw();

                void __run( hashing::function &H ) const throw();

            private:
                Y_DISABLE_ASSIGN(Coordinate);
            };
        }
    }
}

#endif

