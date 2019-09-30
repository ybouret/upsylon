//! \file
#ifndef Y_GEOMETRY_ISO3D_LOCATION_INCLUDED
#define Y_GEOMETRY_ISO3D_LOCATION_INCLUDED 1

#include "y/geometry/iso3d/coordinate.hpp"
#include "y/hashing/fnv.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            //! logical location
            class Location
            {
            public:
                const Coordinate lower;
                const Coordinate upper;
                ~Location() throw();
                Location( const Coordinate & ) throw(); //!< built from one logical coordinate
                Location( const Coordinate &a, const Coordinate &b) throw() ;//!< a!=b
                Location( const Location &) throw();
                
                class Hasher
                {
                public:
                    hashing::fnv H;

                    Hasher() throw();
                    ~Hasher() throw();
                    size_t operator()(const Location &) throw();
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Hasher);
                };

            private:
                Y_DISABLE_ASSIGN(Location);
            };

        }
    }
}

#endif

