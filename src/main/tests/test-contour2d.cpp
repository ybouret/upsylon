#include "y/geometry/contour2d.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"
#include "y/oxide/field2d.hpp"
#include "y/oxide/workspace.hpp"

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
    
    std::cerr << "sizeof(contour2d::point_)=" << sizeof(contour2d::point_) << std::endl;
    std::cerr << "sizeof(contour2d::segment)=" << sizeof(contour2d::segment) << std::endl;

    
    {
        const unit_t                     resolution = 10;
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
        }



        contour::levels z;
        z.insert(0);
        z.insert(-0.1);
        z.insert(-0.1);
        z.insert(1);
        std::cerr << "z=" << z << std::endl;

        //contour2d::unique_points_levels db;

        contour2d::scan(V, x.lower, x.upper, y.lower, y.upper, x, y, z);

    }
}
Y_UTEST_DONE()

