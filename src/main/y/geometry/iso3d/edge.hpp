//! \file
#ifndef Y_GEOMETRY_ISO3D_EDGE_INCLUDED
#define Y_GEOMETRY_ISO3D_EDGE_INCLUDED 1

#include "y/geometry/iso3d/coordinate.hpp"
#include "y/hashing/fnv.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            //! logical edge
            class Edge
            {
            public:
                const Coordinate lower;
                const Coordinate upper;
                ~Edge() throw();
                Edge( const Coordinate & ) throw(); //!< built from one logical coordinate
                Edge( const Coordinate &a, const Coordinate &b) throw() ;//!< a!=b
                Edge( const Edge &) throw();
                
                class Hasher
                {
                public:
                    hashing::fnv H;

                    Hasher() throw();
                    ~Hasher() throw();
                    size_t operator()(const Edge &) throw();
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Hasher);
                };

            private:
                Y_DISABLE_ASSIGN(Edge);
            };

        }
    }
}

#endif

