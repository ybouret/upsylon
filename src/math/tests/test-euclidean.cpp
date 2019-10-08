#include "y/math/euclidean/segment.hpp"
#include "y/utest/run.hpp"
#include "y/associative/set.hpp"

using namespace upsylon;
using namespace math;
using namespace Euclidean;


namespace {

    template <typename T,template <class> class VTX>
    struct euclidean_test
    {
        Y_EUCLIDEAN_POINT_TYPES();
        Y_EUCLIDEAN_NODE_TYPES();

        typedef set<PointKey,SharedPoint> Points;

        static inline
        void make(const char *TID, const char *PID, const size_t nn)
        {
            std::cerr << PID << "<" << TID << ">" << std::endl;

            const size_t np = (nn>0) ? nn : 10 + alea.leq(30);
            std::cerr << "With " << np << " points" << std::endl;

            const_type noise(0.1);
            Points     points(np,as_capacity);
            for(size_t i=0;i<np;++i)
            {
                const_type theta = (numeric<mutable_type>::two_pi * i)/np;
                const_type rho   = fabs_of( 2 * cos_of( theta+theta ) );
                const_type x     = rho * cos_of(theta) + noise * alea.symm<T>();
                const_type y     = rho * sin_of(theta) + noise * alea.symm<T>();
                const_type z     = sin_of(theta/2) + noise*alea.symm<T>();
                const_type arr[4] = {x,y,z,0};
                const_vertex &v = aliasing::map<const_vertex>(arr);
                std::cerr << "v=" << v << std::endl;
                const SharedPoint p = new PointType(v);
                Y_ASSERT( points.insert(p) );
                const SharedNode  n = new NodeType(p);
            }



        }

    };

}

#define _EUCLIDEAN(TYPE,POINT) euclidean_test<TYPE,POINT>::make(#TYPE,#POINT,nn)

#include "y/string/convert.hpp"

Y_UTEST(euclidean)
{

    size_t nn = 0;
    if(argc>1)
    {
        nn = string_convert::to<size_t>( argv[1], "nn" );
    }
#if 1
    _EUCLIDEAN(float,point2d);
    _EUCLIDEAN(double,point2d);

    _EUCLIDEAN(float,point3d);
    _EUCLIDEAN(double,point3d);
#endif

}
Y_UTEST_DONE()



