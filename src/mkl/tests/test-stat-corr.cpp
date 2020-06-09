#include "y/mkl/stat/correlation.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;

namespace {

    template <typename T> static inline
    void doCorr()
    {
        correlation<T> corr;

        for(size_t i=2+alea.leq(100);i>0;--i)
        {
            const T x = alea.symm<T>() * 5;
            const T y = -T(0.1) + T(4.2) * x + alea.symm<T>();
            corr.add(x,y);
        }
        std::cerr << "corr=" << corr.compute() << std::endl;
    }

}

Y_UTEST(correlation)
{
    doCorr<float>();
    doCorr<double>();
    
}
Y_UTEST_DONE()


