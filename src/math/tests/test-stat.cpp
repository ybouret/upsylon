#include "y/math/stat/metrics.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(stat)
{
    for(size_t iter=0;iter<16;++iter)
    {
        {
            math::correlation<double> corr;

            for(size_t i=2+alea.leq(100);i>0;--i)
            {
                const double x = alea.symm<double>() * 5;
                const double y = -0.1 + 4.2 *x + alea.symm<double>();
                corr.add(x,y);
            }
            std::cerr << "corr=" << corr.compute() << std::endl;
        }

        {
            list<float> X;
            for(size_t i=2+alea.leq(100);i>0;--i)
            {
                const float x = alea.to<float>();
                X.push_back(x);
            }
            float sig = 0;
            float ave = math::average_of(X,&sig);
            std::cerr << "ave=" << ave << ", sig=" << sig << std::endl;

            float adev = 0;
            float med  = math::median_of(X,&adev);
            std::cerr << "med=" << med << ", adev=" << adev << std::endl;

        }
    }


    
}
Y_UTEST_DONE()


