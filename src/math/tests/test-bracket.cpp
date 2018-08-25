#include "y/math/opt/bracket.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename T> inline
    T F(T x)
    {
        return cos_of(x);
    }

    template <typename T>
    void do_test()
    {
        triplet<T> x = { 0,1.5, -1 };
        triplet<T> f = { F(x.a), F(x.b), -1 };
        bracket::expand(F<T>,x,f);
        std::cerr << "x=" << x << std::endl;
        std::cerr << "f=" << f << std::endl;
    }
}

Y_UTEST(bracket)
{
    do_test<float>();
    
}
Y_UTEST_DONE()


