#include "y/ptr/cblock.hpp"
#include "y/utest/run.hpp"

#include "y/string.hpp"

using namespace upsylon;

namespace
{
    template <typename T,typename ALLOCATOR>
    void do_test()
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const size_t required = alea.leq(1000);
            memblk<T,ALLOCATOR> blk( required );
            Y_ASSERT(blk.count==required);
        }
    }
    
    template <typename T>
    void do_tests()
    {
        do_test<T,memory::global>();
    }
}

Y_UTEST(memblk)
{
   
    do_tests<char>();
    do_tests<double>();
    do_tests<string>();

}
Y_UTEST_DONE()

