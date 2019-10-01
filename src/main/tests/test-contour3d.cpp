#include "y/geometry/iso3d/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"
#include "y/ptr/auto.hpp"
#include "y/oxide/grid/rectilinear.hpp"
#include "y/oxide/field3d.hpp"

using namespace upsylon;
using namespace geometry;
using namespace Iso3D;
using namespace Oxide;

static inline
const Coordinate & getCoord( const array<Coordinate> &arr )
{
    return arr[ alea.range<size_t>(1,arr.size()) ];
}

static inline Edge *getEdge( const array<Coordinate> &arr )
{
    if( alea.choice() )
    {
        return new Edge( getCoord(arr) );
    }
    else
    {
        const Coordinate &a = getCoord(arr);
        for(;;)
        {
            const Coordinate &b = getCoord(arr);
            if(a==b) continue;
            return new Edge(a,b);
        }
    }

}

#include "y/counting/perm.hpp"

Y_UTEST(contour3d)
{

    {
        std::cerr << "Testing Basic Types..." << std::endl;
        const size_t NC = 1024;
        vector<Coordinate> coords;
        const unit_t m = 5;
        for(size_t i=NC;i>0;--i)
        {
            const Coordinate c(alea.range<unit_t>(-m,m),
                               alea.range<unit_t>(-m,m),
                               alea.range<unit_t>(-m,m));
            coords.push_back(c);
        }
        std::cerr << "#coords=" << coords.size() << std::endl;
        unique(coords);
        std::cerr << "#coords=" << coords.size() << std::endl;

        for(size_t i=NC*16;i>0;--i)
        {
            auto_ptr<Edge> pa = getEdge(coords);
            auto_ptr<Edge> pb = 0;
            do
            {
                pb = getEdge(coords);
            }
            while( *pb == *pa );

            auto_ptr<Edge> pc = 0;
            do {
                pc = getEdge(coords);
            }
            while( *pc == *pa || *pc == *pb );

            const Edge3 org3( *pa, *pb, *pc );

            permutation perm(3);
            for( perm.start(); perm.valid(); perm.next() )
            {
                const Edge3 prm3( *org3.edge[ perm(0) ], *org3.edge[ perm(1) ],*org3.edge[ perm(2) ] );
                Y_ASSERT(prm3==org3);
            }

        }

        std::cerr << "sizeof(Iso3D::Facet)=" << sizeof(Iso3D::Facet) << std::endl;
        std::cerr << "sizeof(Iso3D::Edge3)=" << sizeof(Iso3D::Edge3) << std::endl;
    }

    {
        typedef VertexFor<Coord3D,double>::Type vertex;
        const Layout3D                  L( Coord3D(1,1,1), Coord3D(10,20,30) );
        Field3D<double>                 S( "S", L);
        Field3D<double>                 P( "P", L);
        RectilinearGrid<Coord3D,double> G(L,NULL);
        G.mapRegular(L,vertex(-1,-1,-1),vertex(1,1,1));

        Layout3D::Loop loop( L.lower, L.upper );

        for( loop.start(); loop.valid(); loop.next() )
        {
            const Coord3D &c = loop.value;
            const vertex   v = G(c);
            const double   x = v.x;
            const double   E = exp( -1.4* v.norm2() );

            S(c) = E;
            P(c) = 2*E*x;

        }

        Contour::Levels w;
        w.insert(0.5);
        w.insert(0.7);

        Scanner::Run(S,
                     L.lower.x,L.upper.x,
                     L.lower.y,L.upper.y,
                     L.lower.z,L.upper.z,
                     G[0], G[1], G[2],
                     w);


    }







}
Y_UTEST_DONE()

