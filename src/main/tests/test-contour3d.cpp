#include "y/geometry/iso3d/scanner.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"
#include "y/ptr/auto.hpp"
#include "y/oxide/grid/rectilinear.hpp"
#include "y/oxide/field3d.hpp"
#include "y/ios/ocstream.hpp"

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

        std::cerr << "sizeof(Iso3D::Facet)=" << sizeof(Iso3D::Facet_) << std::endl;
        std::cerr << "sizeof(Iso3D::Edge3)=" << sizeof(Iso3D::Edge3) << std::endl;
    }

    {
        unit_t resolution = 10;
        typedef VertexFor<Coord3D,double>::Type vertex;
        const Layout3D                  L( Coord3D(1,1,1), Coord3D(resolution,2*resolution,3*resolution) );
        Field3D<double>                 S( "S", L);
        Field3D<double>                 P( "P", L);
        RectilinearGrid<Coord3D,double> G(L,NULL);
        G.mapRegular(L,vertex(-1,-1,-1),vertex(1,1,1));

#if 0
        {
            std::cerr << "X=";
            for(unit_t i=G[0].lower;i<=G[0].upper;++i)
            {
                std::cerr << ' ' << G[0][i];
            } std::cerr << std::endl;
        }
        
        {
            std::cerr << "Y=";
            for(unit_t i=G[1].lower;i<=G[1].upper;++i)
            {
                std::cerr << ' ' << G[1][i];
            } std::cerr << std::endl;
        }
        
        {
            std::cerr << "Z=";
            for(unit_t i=G[2].lower;i<=G[2].upper;++i)
            {
                std::cerr << ' ' << G[2][i];
            } std::cerr << std::endl;
        }
#endif
        
        Layout3D::Loop loop( L.lower, L.upper );

        for( loop.start(); loop.valid(); loop.next() )
        {
            const Coord3D &c = loop.value;
            const vertex   v = G(c);
            const double   x = v.x;
            const double   E = exp( -1.4* v.norm2() );

            S(c) = E;
            P(c) = 2*E*x*x;

        }

        {
            vtk &VTK = vtk::instance();
            ios::ocstream fp("s.vtk");
            VTK.writeHeader(fp);
            VTK.writeTitle(fp, "spherical");
            G.write(VTK, fp, S);
            VTK.writePointData(fp,S);
            VTK.writeField(fp,S,S);
        }

        {
            vtk &VTK = vtk::instance();
            ios::ocstream fp("p.vtk");
            VTK.writeHeader(fp);
            VTK.writeTitle(fp, "principal");
            G.write(VTK, fp, P);
            VTK.writePointData(fp,P);
            VTK.writeField(fp,P,P);
        }



        Contour::Levels w;
        w.insert(0.5);
        w.insert(0.7);

        Surfaces surfaces;

        Scanner::Run(surfaces,
                     S,
                     L.lower.x,L.upper.x,
                     L.lower.y,L.upper.y,
                     L.lower.z,L.upper.z,
                     G[0], G[1], G[2],
                     w);

        std::cerr << "#surfaces=" << surfaces.size() << std::endl;
        for(Surfaces::iterator it=surfaces.begin();it!=surfaces.end();++it)
        {
            const Surface_ &s = **it;
            std::cerr << "|_S[" << s.index << "]@" << w[s.index] << std::endl;
            std::cerr << " |_#points=" << s.size() << std::endl;

            const string  fn = vformat("s%u.dat", unsigned(s.index) );
            ios::ocstream fp(fn);
            for(Points::const_iterator p=s.begin(); p != s.end(); ++p )
            {
                fp("%g %g %g\n", (*p)->position.x, (*p)->position.y, (*p)->position.z);
            }

        }
        
        
        Scanner::Run(surfaces,
                     P,
                     L.lower.x,L.upper.x,
                     L.lower.y,L.upper.y,
                     L.lower.z,L.upper.z,
                     G[0], G[1], G[2],
                     w);
        
        std::cerr << "#surfaces=" << surfaces.size() << std::endl;
        for(Surfaces::iterator it=surfaces.begin();it!=surfaces.end();++it)
        {
            const Surface_ &s = **it;
            std::cerr << "|_S[" << s.index << "]@" << w[s.index] << std::endl;
            std::cerr << " |_#points=" << s.size() << std::endl;
            
            const string  fn = vformat("p%u.dat", unsigned(s.index) );
            ios::ocstream fp(fn);
            for(Points::const_iterator p=s.begin(); p != s.end(); ++p )
            {
                fp("%g %g %g\n", (*p)->position.x, (*p)->position.y, (*p)->position.z);
            }
            
        }
    }







}
Y_UTEST_DONE()

