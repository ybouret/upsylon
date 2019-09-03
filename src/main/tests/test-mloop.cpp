#include "y/counting/mloop.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

template <typename T>
void do_mloop(const size_t dim, const T *lo, const T *hi)
{
    mloop<T> loop(dim,lo,hi);

}

Y_UTEST(mloops)
{

    {
        uint8_t lo[] = {0};
        uint8_t hi[] = {0};
        do_mloop<uint8_t>(1,lo,hi);
    }


}
Y_UTEST_DONE()
