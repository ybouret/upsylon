#include "y/mkl/trigconv.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(trigo)
{

    for(double theta=-3.14;theta<=3.14; theta += 0.1 )
    {
        point2d<double> v(cos(theta),sin(theta));

        v *= 0.01 + alea.to<double>();
        const double a = mkl::argument(v);
        std::cerr << "theta=" << theta << " => " << a << std::endl;

    }

}
Y_UTEST_DONE()



