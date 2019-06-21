#include "y/math/signal/curve.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

template <typename T,template <class> class POINT>
static inline void do_curve(const size_t np)
{
    std::cerr << "type  = <" << typeid(T).name() << ">" << std::endl;
    std::cerr << "point = <" << typeid(POINT<T>).name() << ">" << std::endl;
    std::cerr << "\tsizeof(NODE)=" << sizeof( curve::node<T,POINT> ) << std::endl;

    const string root = vformat("curve%ud%u.dat", unsigned(curve::node<T,POINT>::DIM), unsigned(sizeof(T)*8) );

    curve::points<T,POINT> C;

    for(size_t i=0;i<np;++i)
    {
        const T theta = ( numeric<T>::two_pi * i) / np;
        const T x     = 5*cos_of(theta);
        const T y     = 5*sin_of(theta);
        C.add(x,y);
    }
    std::cerr << "#nodes=" << C.size() << std::endl;
    C.update( curve::standard );

    {
        const string  fn = "std-" + root;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            const curve:: node<T,POINT> &node = C[i];
            POINT<T>        p = node.r;
            const POINT<T> &t = node.t;
            fp("%g %g\n",p.x,p.y);
            p += t;
            fp("%g %g\n",p.x,p.y);
            fp << '\n';
        }
    }

    C.update( curve::periodic );

    {
        const string  fn = "per-" + root;
        ios::ocstream fp(fn);
        for(size_t i=1;i<=np;++i)
        {
            const curve:: node<T,POINT> &node = C[i];
            POINT<T>        p = node.r;
            const POINT<T> &t = node.t;
            fp("%g %g\n",p.x,p.y);
            p += t;
            fp("%g %g\n",p.x,p.y);
            fp << '\n';
        }
    }

   

    std::cerr << std::endl;
}

#include "y/string/convert.hpp"
Y_UTEST(curve)
{
    size_t np = 3;
    if(argc>1)
    {
        np = string_convert::to<size_t>(argv[1],"np");
    }
    do_curve<float,point2d>(np);
    do_curve<double,point2d>(np);
    do_curve<float,point3d>(np);
    do_curve<double,point3d>(np);
}
Y_UTEST_DONE()


