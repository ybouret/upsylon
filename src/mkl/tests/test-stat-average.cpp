#include "y/math/stat/average.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;

namespace {

    template <typename T> static inline
    void doAverage()
    {

        list<T> X;
        for(size_t i=2+alea.leq(100);i>0;--i)
        {
            const T x = support::get<T>();
            X.push_back(x);
        }

        T sig1 = 0;
        T ave1 = math::average_of(X,&sig1);
        T sig2 = 0;
        T ave2 = math::average_for(X,&sig2);
        std::cerr << "ave1=" << ave1 << ", sig1=" << sig1 << std::endl;
        std::cerr << "ave2=" << ave2 << ", sig2=" << sig2 << std::endl;

        Y_ASSERT(math::fabs_of(ave1-ave2)<=0);
        Y_ASSERT(math::fabs_of(sig1-sig2)<=0);



    }

}

Y_UTEST(average)
{

    doAverage<float>();
    doAverage<double>();
    
}
Y_UTEST_DONE()


