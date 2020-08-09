#include "y/ptr/memblk.hpp"
#include "y/utest/run.hpp"

#include "y/string.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    void do_test()
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const size_t required = alea.leq(1000);
            memblk<T>    blk( required );
            Y_ASSERT(blk.count==required);
        }
    }
}

Y_UTEST(memblk)
{
    do_test<char>();
    do_test<double>();
    do_test<string>();
    
}
Y_UTEST_DONE()

