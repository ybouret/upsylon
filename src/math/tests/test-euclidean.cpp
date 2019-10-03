#include "y/math/euclidean/arc.hpp"
#include "y/type/complex.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;
using namespace Euclidean;

namespace {

    template <typename T, template <class> class POINT>
    struct euclidean_test
    {
        Y_EUCLIDEAN_POINT_ARGS();     //!< aliases
        Y_EUCLIDEAN_SEGMENT_ARGS();   //!< aliases
        Y_EUCLIDEAN_POINTNODE_ARGS(); //!< aliases

        typedef Segments<T,POINT>           SegmentsType;

        static inline void make()
        {
            const std::type_info &tid = typeid(T);
            const std::type_info &pid = typeid(PointType);

            std::cerr << "Using " << pid.name() << "/" << tid.name() << std::endl;
            std::cerr << "|_sizeof(type)     = " << sizeof(T)           << std::endl;
            std::cerr << "|_sizeof(point)    = " << sizeof(PointType)   << std::endl;
            std::cerr << "|_sizeof(shared)   = " << sizeof(SharedPoint) << std::endl;
            std::cerr << "|_sizeof(node)     = " << sizeof(NodeType)    << std::endl;
            std::cerr << "|_sizeof(list)     = " << sizeof(NodeList)    << std::endl;
            std::cerr << "|_sizeof(segment)  = " << sizeof(SegmentType) << std::endl;

            NodeList points;
            for(size_t i=10+alea.leq(100);i>0;--i)
            {
                SharedPoint P = new PointType();
                points.push_back( new NodeType(P) );
            }

            SegmentsType segments;
            for(size_t i=10+alea.leq(100);i>0;--i)
            {
                const SharedPoint &a = *points.fetch( alea.lt(points.size) );
                const SharedPoint &b = *points.fetch( alea.lt(points.size) );
                segments(a,b);
            }

            segments.update();
        }
    };

}


Y_UTEST(euclidean)
{

    euclidean_test<float,point2d>::make();
    euclidean_test<double,point2d>::make();

    euclidean_test<float, point3d>::make();
    euclidean_test<double,point3d>::make();

    euclidean_test<float,complex>::make();
    euclidean_test<double,complex>::make();


}
Y_UTEST_DONE()



