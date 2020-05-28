#include "y/tensor/tensor1d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;


namespace {

    template <typename T>
    static inline void doTest()
    {
        tensor1d<T> t( 1+alea.leq(10) );
        support::fill1D(t);
    }
}

Y_UTEST(tensors)
{
    doTest<float>();
    doTest<int>();
    doTest<double>();
    doTest<string>();

}
Y_UTEST_DONE()

