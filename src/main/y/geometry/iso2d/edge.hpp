
//! \file
#ifndef Y_GEOM_ISO2D_EDGE_INCLUDED
#define Y_GEOM_ISO2D_EDGE_INCLUDED 1

#include "y/geometry/iso2d/coordinate.hpp"
#include "y/hashing/fnv.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            //__________________________________________________________________
            //
            //! logical edge: a set of (one or) two points, acting as a key
            //__________________________________________________________________
            class Edge
            {
            public:
                const Coordinate lower; //!< lower by lexicographic
                const Coordinate upper; //!< upper by lexicographic

                ~Edge() throw();                                        //!< cleanup
                Edge(const Edge &) throw();                             //!< copy
                Edge(const Coordinate &single) throw();                 //!< lower=upper=single
                Edge(const Coordinate &a, const Coordinate &b) throw(); //!< a!=b, then ordered

                friend bool operator==(const Edge&,const Edge&) throw(); //!< testing same values
                friend bool operator!=(const Edge&,const Edge&) throw(); //!< testing different values

                //! dedicated hasher
                class Hasher
                {
                public:
                    hashing::fnv H;    //!< hashing function
                    Hasher() throw();  //!< setup
                    ~Hasher() throw(); //!< cleanup

                    //! get hash code
                    size_t operator()( const Edge & ) throw();

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
