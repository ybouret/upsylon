#include "y/math/geometric/points.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/list.hpp"
#include "y/memory/pooled.hpp"

using namespace upsylon;
using namespace math;

template <typename T,
template <typename> class POINT,
template <typename> class SEQUENCE>
static inline void do_points()
{
    typedef Geometric::Points<T,POINT,SEQUENCE>  PointsType;
    typedef Geometric::PointInfoFor<T,POINT>     PointInfo;
    typedef typename PointInfo::Type             PointType;
    typedef typename PointInfo::Core             CorePoint;
    
    PointsType points;
    for(typename PointsType::iterator i=points.begin(); i != points.end(); ++i)
    {
        const PointType &p = *i;
        std::cerr << p << std::endl;
    }

}

#define DO_POINTS__(TYPE,SEQ)      \
do_points<TYPE,point2d,SEQ>(); \
do_points<TYPE,complex,SEQ>(); \
do_points<TYPE,point3d,SEQ>()

#define DO_POINTS_(SEQ) \
DO_POINTS__(float,SEQ); DO_POINTS__(double,SEQ)



Y_UTEST(points)
{
    DO_POINTS_(list);
    DO_POINTS_(vector_for<memory::global>::device);
    DO_POINTS_(vector_for<memory::pooled>::device);

}
Y_UTEST_DONE()


