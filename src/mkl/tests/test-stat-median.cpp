#include "y/mkl/stat/median.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/list.hpp"


using namespace upsylon;

namespace {

    template <typename T> static inline
    void doMedian()
    {

        list<T> X;
        for(size_t i=2+alea.leq(100);i>0;--i)
        {
            const T x = support::get<T>();
            X.push_back(x);
        }

        T adv1 = 0;
        T med1 = math::median_of(X,&adv1);
        T adv2 = 0;
        T med2 = math::median_for(X,&adv2);
        std::cerr << "med1=" << med1 << ", adv1=" << adv1 << std::endl;
        std::cerr << "med2=" << med2 << ", adv2=" << adv2 << std::endl;
        Y_ASSERT( math::fabs_of(med1-med2) <= 0 );
        Y_ASSERT( math::fabs_of(adv1-adv2) <= 0 );

    }

}

Y_UTEST(median)
{

    doMedian<float>();
    doMedian<double>();


}
Y_UTEST_DONE()


