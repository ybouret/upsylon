#include "y/geometry/iso3d/facets.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace geometry;
using namespace Iso3D;


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
#define NC 1024

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
Y_UTEST_DONE()

