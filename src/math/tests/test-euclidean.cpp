#include "y/math/euclidean/point.hpp"
#include "y/type/complex.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;
using namespace Euclidean;

namespace {

    template <typename T, template <class> class POINT>
    static inline void make_euclidean()
    {

        typedef Point<T,POINT>              PointType;
        typedef typename PointType::Pointer SharedPoint;
        typedef Node<T,POINT>               NodeType;
        typedef typename NodeType::List     List;

        const std::type_info &tid = typeid(T);
        const std::type_info &pid = typeid(PointType);

        std::cerr << "Using " << pid.name() << "/" << tid.name() << std::endl;
        std::cerr << "|_sizeof(type)   = " << sizeof(T)           << std::endl;
        std::cerr << "|_sizeof(point)  = " << sizeof(PointType)   << std::endl;
        std::cerr << "|_sizeof(shared) = " << sizeof(SharedPoint) << std::endl;
        std::cerr << "|_sizeof(node)   = " << sizeof(NodeType)    << std::endl;
        std::cerr << "|_sizeof(list)   = " << sizeof(List)        << std::endl;

        List points;
        for(size_t i=10+alea.leq(100);i>0;--i)
        {
            SharedPoint P = new PointType();
            points.push_back( new NodeType(P) );
        }

    }

}


Y_UTEST(euclidean)
{

    make_euclidean<float,point2d>();
    make_euclidean<double,point2d>();
    make_euclidean<float,point3d>();
    make_euclidean<double,point3d>();
    make_euclidean<float,complex>();
    make_euclidean<double,complex>();

}
Y_UTEST_DONE()



