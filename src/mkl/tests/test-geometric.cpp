#include "y/mkl/geometric/point.hpp"
#include "y/utest/run.hpp"
#include "y/associative/hash/set.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;
using namespace Geometric;

namespace {

    template <typename T, template <class> class VTX>
    static inline void testPoints()
    {

        typedef Point<T,VTX>                PointType;
        typedef typename PointType::Pointer PointerType;
        typedef typename PointType::vertex  vertex;

        std::cerr << "points: " << type_name_of<vertex>() << std::endl;
        std::cerr << "  dims: " << PointType::Dimensions << std::endl;

        hash_set<PointKey,PointerType> points;

        for(size_t i=10+alea.leq(10);i>0;--i)
        {
            const PointerType p = new PointType( support::get<vertex>() );
            Y_ASSERT(points.insert(p));
        }
        std::cerr << points << std::endl;

    }

}

Y_UTEST(geometric)
{

    testPoints<float,point2d>();
    testPoints<float,point3d>();
    testPoints<float,complex>();

    testPoints<double,point2d>();
    testPoints<double,point3d>();
    testPoints<double,complex>();

}
Y_UTEST_DONE()

