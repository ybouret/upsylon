#include "y/math/signal/curve.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;


template <typename T>
static inline void show_info()
{
    std::cerr << "sizeof=" << sizeof(T) << std::endl;
    std::cerr << "\ttype1d: " <<  curve::info_for<T>::dim << std::endl;
    std::cerr << "\ttype2d: " <<  curve::info_for< point2d<T> >::dim << std::endl;
    std::cerr << "\ttype3d: " <<  curve::info_for< point3d<T> >::dim << std::endl;
    Y_ASSERT(sizeof(T)==sizeof(typename curve::info_for<T>::real));
}

static inline void handle_curve( curve::interface &C )
{
    std::cerr << "#points=" << C.size() << std::endl;
}

Y_UTEST(curve)
{
    show_info<float>();
    show_info<double>();

    curve::points< float >          C1f;
    curve::points< point2d<float> > C2f;
    curve::points< point3d<float> > C3f;

    curve::points< double >          C1d;
    curve::points< point2d<double> > C2d;
    curve::points< point3d<double> > C3d;

    for(size_t i=10+alea.leq(10);i>0;--i)
    {
        {
            const float x = alea.symm<float>();
            const float y = alea.symm<float>();
            const float z = alea.symm<float>();

            C1f.add(x);
            C2f.add(x,y);
            C3f.add(x,y,z);

            C1d.add(x);
            C2d.add(x,y);
            C3d.add(x,y,z);
        }
        
    }

    handle_curve(C1f);
    handle_curve(C2f);
    handle_curve(C3f);

    handle_curve(C1d);
    handle_curve(C2d);
    handle_curve(C3d);


}
Y_UTEST_DONE()

