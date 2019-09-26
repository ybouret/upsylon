#include "y/geometry/contour2d.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/unique.hpp"
#include "y/oxide/field2d.hpp"

using namespace upsylon;
using namespace geometry;

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
    std::cerr << "sizeof(contour2d::point)=" << sizeof(contour2d::point) << std::endl;

    {
        const unit_t resolution = 10;

        Oxide::Field2D<float> V("V", Oxide::Coord2D(-resolution,-resolution), Oxide::Coord2D(resolution,resolution) );
        const Oxide::Layout1D Lx = V.projectOn(0);
        const Oxide::Layout1D Ly = V.projectOn(1);

        contour::levels z;
        z.insert(0);
        z.insert(-0.1);
        z.insert(-0.1);
        z.insert(1);
        std::cerr << "z=" << z << std::endl;

    }
}
Y_UTEST_DONE()

