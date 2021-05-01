#include "y/yap/rational.hpp"
#include "y/mkl/kernel/cyclic.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/rtti.hpp"

using namespace upsylon;
using namespace mkl;

template <typename T>
static inline void do_cyclic(const bool   check,
                             const size_t nmax=10,
                             const size_t niter=4)

{
    (void)check;
    for(size_t n=3;n<=nmax;++n)
    {
        cyclic<T> C(n,3);
        array<T>  &r = C[0];
        array<T>  &u = C[1];
        array<T>  &x = C[2];
        std::cerr << "cyclic<" << n << "> for <" << rtti::name_of<T>() << ">" << std::endl;

        if(n==3)
        {
            std::cerr << "\t#count      = " << C.count      << std::endl;
            std::cerr << "\t#extraneous = " << C.extraneous << std::endl;
        }

        for(size_t jter=0;jter<niter;++jter)
        {
            for(size_t i=C.size();i>0;--i)
            {
                const T a = support::get<T>();
                const T c = support::get<T>();
                const T b = T(1) + fabs_of(a) + fabs_of(c);
                C.a[i] = a;
                C.b[i] = b;
                C.c[i] = c;


            }

            for(size_t iter=0;iter<niter;++iter)
            {
                for(size_t i=C.size();i>0;--i)
                {
                    u[i] = x[i] = 0;
                    r[i] = support::get<T>();
                }
                C.solve(u,r);
                C.mul(x,u);
                tao::sub(x,r);
                std::cerr << "/" << tao::mod2<T>::of(x);
            }
        }
        std::cerr << std::endl;
    }
}

Y_UTEST(cyclic)
{

    do_cyclic<float>(false);
    do_cyclic<double>(false);
    do_cyclic< complex<float> >(false);
    do_cyclic< complex<double> >(false);
    do_cyclic< apq >(true,6,1);

}
Y_UTEST_DONE()


