#include "y/geometry/iso3d/points.hpp"
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
    vector<Coordinate> coords;
    const unit_t m = 5;
    for(size_t i=1000;i>0;--i)
    {
        const Coordinate c(alea.range<unit_t>(-m,m),
                           alea.range<unit_t>(-m,m),
                           alea.range<unit_t>(-m,m));
        coords.push_back(c);
    }
    std::cerr << "#coords=" << coords.size() << std::endl;
    unique(coords);
    std::cerr << "#coords=" << coords.size() << std::endl;
    for(size_t i=1000;i>0;--i)
    {
        auto_ptr<Edge> pa = getEdge(coords);
        auto_ptr<Edge> pb = getEdge(coords);
        auto_ptr<Edge> pc = getEdge(coords);
        Edge::Sort3(*pa, *pb, *pc);
        
        Edge *etab[3] = { &*pa, &*pb, &*pc };
        const Edge3 org3( *etab[0], *etab[1], *etab[2] );
        Y_ASSERT( etab[0] == &*pa );
        Y_ASSERT( etab[1] == &*pb );
        Y_ASSERT( etab[2] == &*pc );

        permutation perm(3);
        for( perm.start(); perm.valid(); perm.next() )
        {
            //std::cerr << perm << std::endl;
            const size_t i0 = perm[1]-1; assert(i0>=0); assert(i0<3);
            const size_t i1 = perm[2]-1; assert(i1>=0); assert(i1<3);
            const size_t i2 = perm[3]-1; assert(i2>=0); assert(i2<3);

            const Edge3 prm3( *etab[i0], *etab[i1], *etab[i2] );
            Y_ASSERT(prm3==org3);
        }
    }
    

}
Y_UTEST_DONE()

