#include "y/counting/mloop.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

template <typename T>
void do_mloop(const size_t dim, const T *lo, const T *hi)
{
    mloop<T> loop(dim,lo,hi);

    for( loop.start(); loop.valid(); loop.next() )
    {
        std::cerr << loop.index << "\t";
        for(size_t i=0;i<loop.dimensions;++i)
        {
            std::cerr << ":" << int64_t(loop[i]);
        }

        std::cerr << std::endl;
    }

}

Y_UTEST(mloops)
{

    {
        uint8_t lo[] = {0};
        uint8_t hi[] = {0};
        do_mloop<uint8_t>(1,lo,hi);
        lo[0] = 10;
        do_mloop<uint8_t>(1,lo,hi);
    }


}
Y_UTEST_DONE()
