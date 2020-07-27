#include "y/mkl/fcn/zfind.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{
    static inline
    double F( double x )
    {
        return 0.1+cos( x*x );
    }
}

Y_UTEST(zfind)
{
    {
        triplet<double> x = {0, -1, 2};
        triplet<double> f = {F(x.a),-1,F(x.c)};
        std::cerr << "x=" << x << std::endl;
        std::cerr << "f=" << f << std::endl;
        if(zfind::run(F,x,f))
        {
            std::cerr << "F(" << x.b << ")=" << f.b << std::endl;
        }
        else
        {
            std::cerr << "couldn't find zero" << std::endl;
        }
        const double x1 = zfind::get(F,0.0,2.0);
        std::cerr << "x1=" << x1 << std::endl;
        const double x2 = zfind::get(0.12,F,0.0,2.0);
        std::cerr << "x2=" << x2 << std::endl;

    }
}
Y_UTEST_DONE()

#include "y/mkl/fcn/lfind.hpp"
namespace {

    template <typename T>
    void test_lfind()
    {
        static const T      ff[] = { T(-0.1), T(0), T(0.1) };
        static const size_t nn = sizeof(ff)/sizeof(ff[0]);
        triplet<T> x;
        triplet<T> f;

        for(size_t i=0;i<nn;++i)
        {
            f.a = ff[i];
            for(size_t j=0;j<nn;++j)
            {
                f.c = ff[j];
                x.a = alea.symm<T>();
                x.c = alea.symm<T>();
                if(f.a*f.c<=0)
                {
                    Y_CHECK(zfind::linear(x,f));
                }
            }
        }
        return;
        
        for(size_t iter=1000;iter>0;--iter)
        {
            x.a = alea.symm<T>();
            x.c = alea.symm<T>();

            f.a = alea.symm<T>();
            f.c = alea.symm<T>();

            if(f.a*f.c<=0)
            {
                Y_ASSERT(zfind::linear(x,f));
            }
        }

    }
}

Y_UTEST(lfind)
{
    test_lfind<float>();
    test_lfind<double>();
}
Y_UTEST_DONE()
