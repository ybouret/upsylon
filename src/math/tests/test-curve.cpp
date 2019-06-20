#include "y/math/signal/curve.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;

template <typename T,template <class> class POINT>
static inline void do_curve()
{
    std::cerr << "type  = <" << typeid(T).name() << ">" << std::endl;
    std::cerr << "point = <" << typeid(POINT<T>).name() << ">" << std::endl;
    std::cerr << "\tsizeof(NODE)=" << sizeof( curve::node<T,POINT> ) << std::endl;

    curve::points<T,POINT> C;

    for(size_t i=1+alea.leq(10);i>0;--i)
    {
        C << support::get< POINT<T> >();
    }
    std::cerr << "#nodes=" << C.size() << std::endl;
    C.update( curve::standard );
    
    std::cerr << std::endl;
}

Y_UTEST(curve)
{
    do_curve<float,point2d>();
    do_curve<double,point2d>();
    do_curve<float,point3d>();
    do_curve<double,point3d>();
}
Y_UTEST_DONE()


