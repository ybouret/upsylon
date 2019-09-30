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

                static int  Compare(const Edge &lhs, const Edge &rhs) throw();
                static void Sort3(Edge &a0, Edge &a1, Edge &a2) throw();
                friend bool operator==( const Edge &lhs, const Edge &rhs ) throw();
                friend bool operator!=( const Edge &lhs, const Edge &rhs ) throw();
                friend bool operator<(const Edge &lhs, const Edge &rhs) throw();

            private:
                Y_DISABLE_ASSIGN(Edge);
            };
            
            class Edge3
            {
            public:
                const Edge e0,e1,e2;
                
                Edge3(const Edge &,const Edge &, const Edge&) throw();
                ~Edge3() throw();
                Edge3(const Edge3&) throw();
                
                friend bool operator==( const Edge3 &lhs, const Edge3 &rhs ) throw();
                static int  Compare(const Edge3 &lhs, const Edge3 &rhs) throw();
                
            private:
                Y_DISABLE_ASSIGN(Edge3);
            };

        }
    }
}

#endif

