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
        for(size_t iter=0;iter<4;++iter)
        {
            std::cerr << "@iter=" << iter << std::endl;
            triplet<T> x = { 10*alea.to<T>(),10*alea.to<T>(), -1 };
            triplet<T> f = { F(x.a), F(x.b), -1 };
            bracket::expand(F<T>,x,f);
            std::cerr << "\tx=" << x << std::endl;
            std::cerr << "\tf=" << f << std::endl;
        }
    }
}

Y_UTEST(bracket)
{
    do_test<float>();
    
}
Y_UTEST_DONE()


