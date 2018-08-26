
#include "y/math/opt/bracket.hpp"
#include "y/math/opt/minimize.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    template <typename T> inline
    T F(T x)
    {
        return cos_of(x*x*0.3);
    }

    template <typename T>
    void do_test()
    {
        for(size_t iter=0;iter<4;++iter)
        {
            std::cerr << "expand@iter=" << iter << std::endl;
            const T ini = 10*alea.symm<T>();
            triplet<T> x = { ini,ini+1, -1 };
            triplet<T> f = { F(x.a), F(x.b), -1 };
            bracket::expand(F<T>,x,f);
            std::cerr << "\tx=" << x << std::endl;
            std::cerr << "\tf=" << f << std::endl;
            std::cerr << std::endl;
            const T xmin = minimize::run(F<T>,x,f);
            const T ymin = F(xmin);
            std::cerr << "F(" << xmin << ")=" << ymin << std::endl;

        }

        {
            std::cerr << "inside@success" << std::endl;
            triplet<T> x = { 1, -1, 4 };
            triplet<T> f = { F(x.a), -1, F(x.c) };
            bracket::inside(F<T>,x,f);
            std::cerr << "\tx=" << x << std::endl;
            std::cerr << "\tf=" << f << std::endl;
            const T xmin = minimize::run(F<T>,x,f);
            const T ymin = F(xmin);
            std::cerr << "F(" << xmin << ")=" << ymin << std::endl;
        }

        {
            std::cerr << "inside@right" << std::endl;
            triplet<T> x = { 0, -1, 1 };
            triplet<T> f = { F(x.a), -1, F(x.c) };
            bracket::inside(F<T>,x,f);
            std::cerr << "\tx=" << x << std::endl;
            std::cerr << "\tf=" << f << std::endl;
            const T xmin = minimize::run(F<T>,x,f);
            const T ymin = F(xmin);
            std::cerr << "F(" << xmin << ")=" << ymin << std::endl;
        }

        {
            std::cerr << "inside@left" << std::endl;
            triplet<T> x = { -1, -1, 1 };
            triplet<T> f = { F(x.a), -1, F(x.c) };
            bracket::inside(F<T>,x,f);
            std::cerr << "\tx=" << x << std::endl;
            std::cerr << "\tf=" << f << std::endl;
            const T xmin = minimize::run(F<T>,x,f);
            const T ymin = F(xmin);
            std::cerr << "F(" << xmin << ")=" << ymin << std::endl;
        }
    }

}


Y_UTEST(min)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()
