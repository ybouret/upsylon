#include "y/geometry/contour2d.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"
#include "y/oxide/field2d.hpp"
#include "y/oxide/workspace.hpp"
#include "y/oxide/vtk.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace geometry;

typedef point2d<float> p2d;

Y_UTEST(contour2d)
{
    {
        vector<contour2d::coordinate> coords;
        for(size_t i=100+alea.leq(1000);i>0;--i)
        {
            const contour2d::coordinate c(alea.range<unit_t>(-10,10),
                                           alea.range<unit_t>(-10,10),
                                           alea.leq(1));
            coords.push_back(c);
        }
        std::cerr << coords << std::endl;
        hsort(coords, contour2d::coordinate::compare);
        std::cerr << coords << std::endl;

        for(size_t i=coords.size();i>1;--i)
        {
            const contour2d::coordinate &lo = coords[i-1];
            const contour2d::coordinate &hi = coords[i];
            Y_ASSERT(lo==lo);
            Y_ASSERT(hi==hi);
            Y_ASSERT(contour2d::coordinate::compare(lo,lo)==0);
            Y_ASSERT(contour2d::coordinate::compare(hi,hi)==0);
            switch(contour2d::coordinate::compare(lo,hi))
            {
                case -1:
                case  0:
                    break;

                default:
                    throw exception("invalid contour2d::coordinate ordering");
            }
        }

        unique(coords);
        std::cerr << coords << std::endl;


        for(size_t iter=10+alea.leq(100);iter>0;--iter)
        {
            const contour2d::coordinate &a = coords[ alea.range<size_t>(1,coords.size()) ];
            const contour2d::coordinate &b = coords[ alea.range<size_t>(2,coords.size()) ];

            contour2d::edge eA(a);
            contour2d::edge eB(b);
            if(a==b)
            {
                Y_ASSERT(eA==eB);
            }
            else
            {
                Y_ASSERT(eA!=eB);
                contour2d::edge eAB(a,b);
                contour2d::edge eBA(b,a);
                Y_ASSERT(eAB==eBA);
            }

        }


    }
    
    unit_t resolution = 15;
    if( argc>1 )
    {
        resolution = string_convert::to<unit_t>(argv[1],"resolution");
    }

    
    {
        const Oxide::Layout2D            fullLayout( Oxide::Coord2D(1,1), Oxide::Coord2D(2*resolution,3*resolution) );
        Oxide::Workspace<Oxide::Coord2D> W(fullLayout,
                                           Oxide::Coord2D(1,1),
                                           0,
                                           Oxide::Coord2D(0,0),
                                           0);


        Oxide::Field2D<float>    & V  = W.create< Oxide::Field2D<float> >("V");
        const Oxide::AxisLayouts & Ax = W.axis[0];
        const Oxide::AxisLayouts & Ay = W.axis[1];

        Oxide::Field1D<float> x("x",Ax.outer);
        Oxide::Field1D<float> y("y",Ay.outer);

        Ax.mapBoundaries(x,-1.0,1.0);
        Ay.mapBoundaries(y,-1.0,1.0);


        // create dipolar field

        p2d p1(-0.5,0);
        p2d p2(0.7,0);

        {
            Oxide::Layout2D::Loop loop(V.lower,V.upper);
            for( loop.start(); loop.valid(); loop.next() )
            {
                const p2d p( x[loop.value.x], y[loop.value.y] );
                const float v1 = 1.0f/(sqrtf((p-p1).norm2())+0.1);
                const float v2 = 1.0f/(sqrtf((p-p2).norm2())+0.1);
                V(loop.value) = v1-v2;
            }
            {
                const string filename = "dipole.vtk";
                ios::ocstream fp(filename);

                const Oxide::vtk &VTK = Oxide::vtk::instance();
                VTK.writeHeader(fp);
                VTK.writeTitle(fp,filename);
                VTK.writeLayout(fp,V);
                VTK.writePointData(fp,V);
                VTK.writeField(fp,V,V);
            }
        }



        contour::levels z;
        z.insert(0);
        z.insert(-0.2);
        z.insert(-0.1);
        z.insert(1);
        z.insert(2);
        z.insert(-3);
        std::cerr << "z=" << z << std::endl;

        contour2d::level_set ls;
        contour2d::scan(ls,V, x.lower, x.upper, y.lower, y.upper, x, y, z);

        {
            ios::ocstream pfp("lsp.dat");
            ios::ocstream sfp("lss.dat");
            for( contour2d::level_set::iterator it = ls.begin(); it != ls.end(); ++it )
            {
                const contour2d::level &L = *it;
                std::cerr << "Level #" << L->index << "@" << z[L->index] << std::endl;
                std::cerr << "    |_#points   = " << L->size() << std::endl;
                std::cerr << "    |_#segments = " << L->slist.size << std::endl;
                for( contour2d::points::const_iterator p = L->begin(); p != L->end(); ++p )
                {
                    const contour2d::vertex &v = (*p)->position;
                    pfp("%.15g %.15g %u\n", v.x, v.y, unsigned(L->index) );
                }

                for( contour2d::segment *s = L->slist.head; s; s=s->next)
                {
                    const contour2d::vertex &a = s->head->position;
                    const contour2d::vertex &b = s->tail->position;
                    sfp("%.15g %.15g %u\n", a.x, a.y, unsigned(L->index) );
                    sfp("%.15g %.15g %u\n", b.x, b.y, unsigned(L->index) );
                    sfp("\n");
                }

            }
        }

    }
}
Y_UTEST_DONE()

