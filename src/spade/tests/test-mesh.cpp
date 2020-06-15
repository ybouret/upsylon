#include "y/spade/mesh/rectilinear.hpp"
#include "y/spade/mesh/curvilinear.hpp"
#include "y/spade/mesh/points.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
    
    template <typename COORD, typename T>
    static inline void doTest()
    {
        const COORD         rng = 30 * Coord::Ones<COORD>();
        const Layout<COORD> L( Coord::Integer(rng,alea), Coord::Integer(rng,alea) );

        typedef typename DenseMesh<COORD,T>::Vertex Vertex;
        typedef typename DenseMesh<COORD,T>::Box    Box;

        Box box( support::get<Vertex>(), support::get<Vertex>() );

        std::cerr << "box=" << box << std::endl;

        {
            RectilinearMesh<COORD,T> rmesh( "rmesh", L );
            std::cerr << rmesh.label << " : " << rmesh.category() << " : " << rmesh << std::endl;
            for(unsigned dim=0;dim<rmesh.space;++dim)
            {
                std::cerr << "\t" << rmesh[dim].name << " : " << rmesh[dim] << std::endl;
                for(Coord1D i=rmesh[dim].lower;i<=rmesh[dim].upper;++i)
                {
                    rmesh[dim][i] = T(i);
                }
            }
            typename Layout<COORD>::Loop loop(rmesh.lower,rmesh.upper);
            for( loop.boot(); loop.good(); loop.next() )
            {
                const COORD    C = loop.value;
                const typename RectilinearMesh<COORD,T>::Vertex v = rmesh( C );
                const T       *p = (const T *)&v;
                for(unsigned dim=0;dim<rmesh.Dimensions;++dim)
                {
                    const Coord1D c = static_cast<Coord1D>(p[dim]);
                    Y_ASSERT(c==Coord::Of(C,dim));
                }
            }

            std::cerr <<"\tmapping to regular box..." << std::endl;
            if( rmesh.isThick() )
            {
                rmesh.mapRegular(box,L);
                Box    B = rmesh.aabb();
                Vertex G(0);
                T      R = rmesh.Rg(G);
                std::cerr << "\taabb  : " << B << std::endl;
                std::cerr << "\tbar   : " << G << std::endl;
                std::cerr << "\tRg    : " << R << std::endl;
                std::cerr << "\tscale : " << rmesh.scaling() << std::endl;
            }
            else
            {
                std::cerr << "!!! rmesh is not thick !!!" << std::endl;
            }
        }
        
        
        {
            CurvilinearMesh<COORD,T> cmesh( "cmesh", L );
            std::cerr << cmesh.label << " : " << cmesh.category() << " : " << cmesh << std::endl;
            for(unsigned dim=0;dim<cmesh.space;++dim)
            {
                std::cerr << "\t" << cmesh[dim].name << " : " << cmesh[dim] << std::endl;
            }
            typename Layout<COORD>::Loop loop(cmesh.lower,cmesh.upper);
            for( loop.boot(); loop.good(); loop.next() )
            {
                const COORD C = loop.value;
                typename CurvilinearMesh<COORD,T>::Vertex v(0);
                for(unsigned dim=0;dim<cmesh.space;++dim)
                {
                    *((T *)&v+dim) = alea.to<T>();
                }
                cmesh(C,v);
                const typename CurvilinearMesh<COORD,T>::Vertex delta = v-cmesh(C);
                for(unsigned dim=0;dim<cmesh.Dimensions;++dim)
                {
                    Y_ASSERT( mkl::fabs_of( *((const T *)&delta+dim) ) <= 0);
                }
            }

            std::cerr <<"\tmapping to regular box..." << std::endl;
            if( cmesh.isThick() )
            {
                cmesh.mapRegular(box,L);
                Box    B = cmesh.aabb();
                Vertex G(0);
                T      R = cmesh.Rg(G);
                std::cerr << "\taabb: " << B << std::endl;
                std::cerr << "\tbar : " << G << std::endl;
                std::cerr << "\tRg  : " << R << std::endl;
              
            }
            else
            {
                std::cerr << "!!! cmesh is not thick !!!" << std::endl;
            }

        }


    }

    template <typename COORD, unsigned SPACE,typename T> static inline
    void doTest2()
    {
        const COORD         rng = 30 * Coord::Ones<COORD>();
        const Layout<COORD> L( Coord::Integer(rng,alea), Coord::Integer(rng,alea) );

        {
            PointMesh<COORD,SPACE,T> pmesh( "pmesh", L );
            std::cerr << pmesh.label << " : " << pmesh.category() << " : " << pmesh << std::endl;
            typedef typename PointMesh<COORD,SPACE,T>::Vertex Vertex;
            std::cerr << "\tvertex=" << type_name_of<Vertex>() << std::endl;

            typename Layout<COORD>::Loop loop(pmesh.lower,pmesh.upper);
            for( loop.boot(); loop.good(); loop.next() )
            {
                const COORD  C = loop.value;
                const Vertex v = support::get<Vertex>();
                pmesh[C] = v;
            }
            
            typedef typename PointMesh<COORD,SPACE,T>::Box Box;
            Box aabb = pmesh.aabb();
            std::cerr << "\taabb: " << aabb << std::endl;
            for( loop.boot(); loop.good(); loop.next() )
            {
                Y_ASSERT(aabb.has( pmesh[*loop] ));
            }
            Vertex G = pmesh.barycenter();
            std::cerr << "\tbar : " << G << std::endl;
            T      R = pmesh.Rg(G);
            std::cerr << "\tbar : " << G << std::endl;
            std::cerr << "\tRg  : " << R << std::endl;

        }
    }

    template <typename T> static inline
    void doTest3D()
    {
        typedef point3d<T>       Vertex;
        typedef Kernel::Box<T,3> Box;

        const Layout3D L( Coord3D(0,0,-10), Coord3D(9,19,10) );

        Box box( support::get<Vertex>(), support::get<Vertex>() );

        std::cerr << "box=" << box << std::endl;

        CurvilinearMesh<Coord3D,T> cmesh( "cmesh3d", L );

        cmesh.mapRegular(box,L);

        


    }

}

#include "y/ios/ocstream.hpp"

Y_UTEST(mesh)
{
    std::cerr << "-- Dense" << std::endl;
    doTest<Coord1D,float>();
    doTest<Coord1D,double>();
    doTest<Coord2D,float>();
    doTest<Coord2D,double>();
    doTest<Coord3D,float>();
    doTest<Coord3D,double>();
    std::cerr << std::endl;

    std::cerr << "-- Loose" << std::endl;
    doTest2<Coord1D,1,float>();
    doTest2<Coord1D,1,double>();

    doTest2<Coord1D,2,float>();
    doTest2<Coord1D,2,double>();

    doTest2<Coord1D,3,float>();
    doTest2<Coord1D,3,double>();

    doTest2<Coord2D,2,float>();
    doTest2<Coord2D,2,double>();

    doTest2<Coord2D,3,float>();
    doTest2<Coord2D,3,double>();

    std::cerr << std::endl;

    std::cerr << "-- Specific" << std::endl;
    doTest3D<float>();
    doTest3D<double>();
    std::cerr << std::endl;

    {
        ios::ocstream fp("disk.dat");
        const size_t nt = 10;
        const size_t nr = 10;
        const double dTheta = mkl::numeric<double>::two_pi / nt;
        const double radius = alea.to<double>();
        for(size_t t=0;t<nt;++t)
        {
            const double theta = dTheta * t;
            for(size_t r=1;r<=nr;++r)
            {
                const double rr = sqrt( radius*radius*r );
                const double x  = rr * cos(theta);
                const double y  = rr * sin(theta);
                fp("%g %g\n",x,y);
            }
        }
    }


}
Y_UTEST_DONE()


