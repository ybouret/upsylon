#include "y/code/decomp.hpp"
#include "y/utest/run.hpp"
#include "y/mpl/rational.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline
    void doDecomp()
    {
        typename decompose<T>::plist l;

    }
}

Y_UTEST(decomp)
{
    doDecomp<int>();
}
Y_UTEST_DONE()

