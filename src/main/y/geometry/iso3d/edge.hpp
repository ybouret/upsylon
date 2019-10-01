//! \file
#ifndef Y_GEOMETRY_ISO3D_EDGE_INCLUDED
#define Y_GEOMETRY_ISO3D_EDGE_INCLUDED 1

#include "y/geometry/iso3d/coordinate.hpp"
#include "y/hashing/fnv.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            //! logical location
            class Edge
            {
            public:
                const Coordinate lower; //!< lower coordinate
                const Coordinate upper; //!< upper coordinate

                ~Edge() throw();                                          //!< cleanup
                Edge( const Coordinate & ) throw();                       //!< built from one logical coordinate
                Edge( const Coordinate &a, const Coordinate &b) throw() ; //!< a!=b
                Edge( const Edge &) throw();                              //!< copy

                //! dedicated hasher
                class Hasher
                {
                public:
                    hashing::fnv H;                          //!< internal hashing function
                    Hasher() throw();                        //!< setup
                    ~Hasher() throw();                       //!< cleanup
                    size_t operator()(const Edge &) throw(); //!< get the key
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Hasher);
                };

                //! compare in lexicographic order of coordinates
                static int  Compare(const Edge &lhs, const Edge &rhs) throw();

                //! sort three edges
                //static void Sort3(Edge &a0, Edge &a1, Edge &a2) throw();

                static void Sort3(Edge **arr) throw();


                //! test component wise equality
                friend bool operator==( const Edge &lhs, const Edge &rhs ) throw();
                //! test component wise difference
                friend bool operator!=( const Edge &lhs, const Edge &rhs ) throw();
                //! lexicographic
                friend bool operator<(const Edge &lhs, const Edge &rhs) throw();

            private:
                Y_DISABLE_ASSIGN(Edge);
            };

            class Edge3
            {
            public:
                const  Edge *edge[3];
                //! with three DIFFERENT and PERSISTENT edges
                Edge3(const Edge &,const Edge &, const Edge&) throw();
                ~Edge3() throw();

                //! will compare addresses, for they are unique
                friend bool operator==( const Edge3 &lhs, const Edge3 &rhs ) throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Edge3);
            };

            
        }
    }
}

#endif

